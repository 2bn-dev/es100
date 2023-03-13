/*! \file
 * \brief Contains information on ES100 Register location and bit positions and functions to validate read and write addresses against datasheet limitations
 */

#ifndef LIBES100_REGISTERS_H
#define LIBES100_REGISTERS_H

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>


// ES100 API register addresses
#define ES100_CONTROL_0_REG      0x0
#define ES100_CONTROL_1_REG      0x1
#define ES100_IRQ_STATUS_REG     0x2
#define ES100_STATUS0_REG        0x3
#define ES100_YEAR_REG           0x4
#define ES100_MONTH_REG          0x5
#define ES100_DAY_REG            0x6
#define ES100_HOUR_REG           0x7
#define ES100_MINUTE_REG         0x8
#define ES100_SECOND_REG         0x9
#define ES100_NEXT_DST_MONTH_REG 0xA
#define ES100_NEXT_DST_DAY_REG   0xB
#define ES100_NEXT_DST_HOUR_REG  0xC
#define ES100_DEVICE_ID_REG      0xD


// ES100 Register bit shifts

#define ES100_CONTROL_0_REG_START           0
#define ES100_CONTROL_0_REG_ANT1_OFF        1
#define ES100_CONTROL_0_REG_ANT2_OFF        2
#define ES100_CONTROL_0_REG_START_ANTENNA   3
#define ES100_CONTROL_0_REG_TRACKING_ENABLE 4

#define ES100_IRQ_STATUS_REG_RX_COMPLETE    0
//RSVD?                                     1
#define ES100_IRQ_STATUS_REG_CYCLE_COMPLETE 2

#define ES100_STATUS_0_REG_RX_OK            0
#define ES100_STATUS_0_REG_ANT              1
//RSVD?                                     2
#define ES100_STATUS_0_REG_LSW0             3
#define ES100_STATUS_0_REG_LSW1             4
#define ES100_STATUS_0_REG_DST0             5
#define ES100_STATUS_0_REG_DST1             6
#define ES100_STATUS_0_REG_TRACKING         7



/*! Validates an I2C address against the allowed read registers in the KXTJ3 Datasheet
 *
 *
 * 
 * \param address i2c register address to validate
 * \returns bool representing if reads are allowed or not
 * \see ES100_VALID_READ_ADDRESSES[]
 */
bool es100_valid_read_address(uint8_t address);

/*! Validates an I2C address against the allowed write registers in the KXTJ3 Datasheet
 *  
 *   
 *     
 *  \param address i2c register address to validate
 *  \returns bool representing if writes are allowed or not
 *  \see ES100_VALID_WRITE_ADDRESSES[]
 */
bool es100_valid_write_address(uint8_t address);
#endif
