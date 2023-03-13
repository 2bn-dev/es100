/*! \file
 * \brief Code for primary kxtj3 library
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "es100.h"
#include "es100_debug.h"
#include "es100_i2c_rp2040.h"

int es100_bcd_to_int(uint8_t input){
    return (input&0xf) + (((input&0xf0)>>4)*10);
}

int es100_send_start_rx(bool ant1_en, bool ant2_en, bool tracking_mode, bool ant2_first){
    uint8_t tx_byte = 0;
    tx_byte |= (0x1             << ES100_CONTROL_0_REG_START);
    tx_byte |= ((!ant1_en)      << ES100_CONTROL_0_REG_ANT1_OFF);
    tx_byte |= ((!ant2_en)      << ES100_CONTROL_0_REG_ANT2_OFF);
    tx_byte |= (tracking_mode   << ES100_CONTROL_0_REG_TRACKING_ENABLE);
    tx_byte |= (ant2_first      << ES100_CONTROL_0_REG_START_ANTENNA);

    uint8_t length = 1;

    es100_i2c_get_txbuf_ptr()[0] = tx_byte;
    int ret = es100_i2c_write_register_blocking_uint8(es100_get_i2c_target_address(), ES100_CONTROL_0_REG, length);
    if(ret < 1)
        _DBG("WRITE ERROR %d", ret);

    return ret;
}

int es100_get_device_id(){
    return es100_i2c_return_register_byte(ES100_DEVICE_ID_REG);
}

int es100_get_timestamp(es100_timestamp *data){
    uint8_t length = 9;
    int ret = es100_i2c_read_register_blocking_uint8(es100_get_i2c_target_address(), ES100_YEAR_REG,  length); // gets all 6 registers
    if (ret < 1)
        return ret;

    data->year   = es100_bcd_to_int(es100_i2c_get_rxbuf_ptr()[ES100_YEAR_REG - ES100_YEAR_REG]);
    data->month  = es100_bcd_to_int(es100_i2c_get_rxbuf_ptr()[ES100_MONTH_REG - ES100_YEAR_REG]);
    data->day    = es100_bcd_to_int(es100_i2c_get_rxbuf_ptr()[ES100_DAY_REG-ES100_YEAR_REG]);
    data->hour   = es100_bcd_to_int(es100_i2c_get_rxbuf_ptr()[ES100_HOUR_REG - ES100_YEAR_REG]);
    data->minute = es100_bcd_to_int(es100_i2c_get_rxbuf_ptr()[ES100_MINUTE_REG - ES100_YEAR_REG]);
    data->second = es100_bcd_to_int(es100_i2c_get_rxbuf_ptr()[ES100_SECOND_REG - ES100_YEAR_REG]);
    data->next_dst_month = es100_bcd_to_int(es100_i2c_get_rxbuf_ptr()[ES100_NEXT_DST_MONTH_REG - ES100_YEAR_REG]);
    data->next_dst_day = es100_bcd_to_int(es100_i2c_get_rxbuf_ptr()[ES100_NEXT_DST_DAY_REG - ES100_YEAR_REG]);
    data->next_dst_hour = es100_bcd_to_int(((es100_i2c_get_rxbuf_ptr()[ES100_NEXT_DST_HOUR_REG - ES100_YEAR_REG]) & 0xf));
    data->dst_special = (((es100_i2c_get_rxbuf_ptr()[ES100_NEXT_DST_HOUR_REG - ES100_YEAR_REG]) & 0xf0)>>4);

    return ret;
}

int es100_get_interrupt_status(){
    return es100_i2c_return_register_byte(ES100_IRQ_STATUS_REG);
}

int es100_get_status(es100_status_0 *data){
    int ret = es100_i2c_return_register_byte(ES100_STATUS0_REG);

    if(ret < 1)
        return ret;

    data->rx_ok = (ret & 0x1);
    data->ant = ((ret & 0x2)>>1);
    data->leap_second_warning = ((ret & 0x18) >> 3);
    data->daylight_savings_time = ((ret & 0x60) >> 5);
    data->tracking = ((ret & 0x80) >> 7);

    //I don't know why this isn't defined in the datasheet... make it easier to use by collapsing.
    if(data->leap_second_warning == LSW_NONE_INVALID)
        data->leap_second_warning = LSW_NONE;

    return ret;
}

void es100_debug_dump(){
    for(int addr = 0x0; addr <= 0xd; addr++){
        int resp = es100_i2c_return_register_byte(addr);
        printf("%d: %02x\n", addr, resp);
    }
}
