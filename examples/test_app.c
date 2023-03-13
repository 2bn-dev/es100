
#include "pico/stdlib.h"
#include "pico/stdio.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"

#include <stdio.h>
#include <stdarg.h>

#include <es100.h>
#include <es100_i2c.h>

//#define DEBUG_ON 1
#include <es100_debug.h>


// Change your I2C pins here:
#define PIN_I2C_SDA 0
#define PIN_I2C_SCL 1
#include <es100_i2c_rp2040.h>

// Change your Enable and IRQ pins here:
#define PIN_ES100_EN  2
#define PIN_ES100_IRQ 3

uint64_t previous_irq_timestamp = 0;

// This was the original application used for development and initial testing.
// it will wait for a USB serial connection.

void fatal_error_infinite_loop(){
	printf("\nExecution Stalled\n");
	while(true){
		_DBG("FATAL ERROR STALL!");
		sleep_ms(1000);
	}
}
void fatal_error(const char* format, ...){

    printf("\n\nFATAL: ");
    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);
    printf("\n\n\n");

	fatal_error_infinite_loop();
}

void warning(const char* format, ...){
	printf("\n\nWARNING: ");
    va_list argptr;
    va_start(argptr, format);
    vprintf(format, argptr);
    va_end(argptr);

    printf("\n\n\n");
}


void my_irq_handler(){
    uint64_t irq_timestamp = time_us_64();

    int interrupt_status = es100_get_interrupt_status();

    if(interrupt_status < 0)
        warning("es100_get_interrupt_status() < 0: %d", interrupt_status);

    //RX Complete == 0x1, "Cycle complete" (unsuccessfully) == 0x4, else = ???
    if(interrupt_status != 0x1)
        return;

    es100_status_0 *status = (es100_status_0*) malloc(sizeof(es100_status_0));
    int status_ret = es100_get_status(status);
    if(status_ret < 0)
        warning("ees100_get_status() < 0: %d", status_ret);

    es100_timestamp *timestamp = (es100_timestamp*) malloc(sizeof(es100_timestamp));
    memset(timestamp, 0x0, sizeof(es100_timestamp));
    int ts_ret = es100_get_timestamp(timestamp);

    if(ts_ret < 0)
        warning("es100_get_timestamp() ret < 0: %d", ts_ret);
    
    printf("IRQ! UTC: 20%02d/%02d/%02d ", timestamp->year, timestamp->month, timestamp->day);
    printf("%02d:%02d:%02d ", timestamp->hour, timestamp->minute, timestamp->second);
    printf("Next_DST(%02d/%02d/%02d) ", timestamp->next_dst_month, timestamp->next_dst_day, timestamp->next_dst_hour);

    printf("DST_Special(");
    if(timestamp->dst_special < 0x8)
        //"No DST Special condiution (Next DST month, day, and hour fields are valid)
        printf(" None ");
    else if(timestamp->dst_special == 0x8)
        //"DST date and time is outside of defined schedule table"
        printf(" ERR_OUTSIDE_TABLE ");
    else if(timestamp->dst_special == 0x9)
        //"DST off (regardless of date)"
        printf(" WARN_DST_OFF ");
    else if(timestamp->dst_special == 0xA)
        //"DST On (regardless of date)"
        printf(" WARN_DST_ON ");
    else
        //Anything else is "reserved" (IE, unknown at this point)
        printf(" WARN_RESERVED_UNK ");
    
    printf(") uS(%" PRIu64 ") ", irq_timestamp);
    printf("uS Period(%" PRIu64 ") ", (irq_timestamp-previous_irq_timestamp));
    printf("irq( %02x ) status( ", interrupt_status);

    if(status->rx_ok == true)
        printf("RX_OK ");
    else
        printf("RX_NOT_OK ");
    
    if(status->ant == true)
        printf("ANT2 ");
    else
        printf("ANT1 ");
    
    if(status->leap_second_warning == LSW_NONE)
        printf("LEAP_SECOND_NONE ");
    else if(status->leap_second_warning == LSW_NEGATIVE)
        printf("LEAP_SECOND_NEGATIVE ");
    else if(status->leap_second_warning == LSW_POSITIVE)
        printf("LEAP_SECOND_POSITIVE ");

    if(status->daylight_savings_time == 0x0)
        printf("DST_NONE ");
    else if(status->daylight_savings_time == 0x2)
        printf("DST_BEGINS_TODAY ");
    else if(status->daylight_savings_time == 0x3)
        printf("DST_IN_EFFECT ");
    else if(status->daylight_savings_time == 0x1)
        printf("DST_ENDS_TODAY ");

    if(status->tracking == true)
        printf("MODE_TRACKING ");
    else
        printf("MODE_NORMAL ");

    printf(")\n");

    free(timestamp);
    free(status);

    int start_status = es100_send_start_rx(true, true, false, false);
    if(start_status < 1)
        fatal_error("ES100 error while trying to send rx start command, status code: %d", start_status);

    previous_irq_timestamp = irq_timestamp;
}

void core1_entry(){
    //Move to second core so we aren't interrupted by USB or uart traffic
    // ... and because it's here!
    // this also allows us to printf within an interrupt without overflowing the stdout buffer.
    es100_i2c_init_local();
    gpio_init(PIN_ES100_EN);
    gpio_init(PIN_ES100_IRQ);

    gpio_set_dir(PIN_ES100_EN, GPIO_OUT);
    gpio_set_dir(PIN_ES100_IRQ, GPIO_IN);

    gpio_set_irq_enabled_with_callback(PIN_ES100_IRQ, GPIO_IRQ_EDGE_FALL, true, my_irq_handler);
    gpio_put(PIN_ES100_EN, 1);
    sleep_ms(100);

    int start_status = es100_send_start_rx(true, true, false, false);
    if(start_status < 1)
        fatal_error("ES100 error while trying to send rx start command, status code: %d", start_status);

    sleep_ms(100);
    
    //It appears the device ID shows up as 0x0 until you send the start command, then it returns 0x10
    int device_id = es100_get_device_id();
    if(device_id != 0x10)
        fatal_error("ES100 not responding / returned invalid device ID (expected 0x10, received 0x%02x)", device_id);

    while(true){
        sleep_ms(1000);
    }
}

int main(){
	stdio_usb_init();
	while(!stdio_usb_connected()){
		sleep_ms(1);
	}
	_DBG("USB Connected");
	sleep_ms(100);

    multicore_reset_core1(); // Need reset for hot-restarts to work right
    multicore_launch_core1(core1_entry);

    //This core will handle USB, uart, and any other code that is added by pico-sdk, the second core will handle nothing but i2c and our IRQ, this maximizes timing accuracy.
	while(true){
		sleep_ms(1000);
	}

}


