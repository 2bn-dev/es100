/*! \file
 * \brief Primary library headers, include this to use the library
 */

#ifndef ES100_H
#define ES100_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

#include "es100_registers.h"
#include "es100_i2c.h"

enum leap_second_warning_messages {
    LSW_NONE = 0,
    LSW_NONE_INVALID,
    LSW_NEGATIVE,
    LSW_POSITIVE
};

enum daylight_savings_time_messages {
    DST_NOT_IN_EFFECT = 0,
    DST_ENDS_TODAY,
    DST_BEGINS_TODAY,
    DST_IS_IN_EFFECT
};


typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t next_dst_month;
    uint8_t next_dst_day;
    uint8_t next_dst_hour;
    uint8_t dst_special;
} es100_timestamp;

typedef struct {
    bool rx_ok;
    bool ant;
    enum leap_second_warning_messages leap_second_warning;
    enum daylight_savings_time_messages daylight_savings_time;
    bool tracking;
} es100_status_0;


/*! Startup configuration
 * \returns status code (value < 0) or length of write (value > 0)
 */
int es100_send_start_rx(bool ant1_en, bool ant2_en, bool tracking_mode, bool ant2_first);


int es100_get_device_id();
int es100_get_interrupt_status();
int es100_get_status();
int es100_get_timestamp(es100_timestamp *data);




#endif
