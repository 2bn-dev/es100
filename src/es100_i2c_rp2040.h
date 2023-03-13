
/*! \file
 * \brief RP2040 specific I2C implementations for reading/writing registers over I2C
 *
 *
 *
 */

#ifndef ES100_I2C_RP2040_H
#define ES100_I2C_RP2040_H

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/i2c.h"


#include "es100_registers.h"
#include "es100_i2c.h"

#ifndef PIN_I2C_SDA
#define PIN_I2C_SDA 0
#endif

#ifndef PIN_I2C_SCL
#define PIN_I2C_SCL 1
#endif

#ifndef I2C_SPEED
#define I2C_SPEED 400*1000
#endif

#ifndef I2C_INTERFACE
#define I2C_INTERFACE i2c0
#endif

#ifndef I2c_READ_TIMEOUT_MS
#define I2C_READ_TIMEOUT_MS 50
#endif 


#ifndef I2C_WRITE_TIMEOUT_MS
#define I2C_WRITE_TIMEOUT_MS 50
#endif


/*!
 * Initialize I2C interface and pull ups
 *
 * Set defines PIN_I2C_SDA and PIN_I2C_SCL to override the I2C Pins.
 *
 */

void es100_i2c_init_local();


/*!
 * RP2040 specific I2C register read (blocking)
 *
 * This function is used internally to read data from the accelerometer via RP2040 I2C hardware interface
 * \param target_addr I2C address command is directed at
 * \param register_addr I2C register address to start reading from
 * \param length of read (MAX I2C_BUF_SIZE)
 *
 * \returns read byte length if successful (value > 0) otherwise status code (value < 0)
 */

int es100_i2c_read_register_blocking_uint8(uint8_t const target_addr, uint8_t const register_addr, uint8_t length);


/*!
 * RP2040 specific I2C register write (blocking)
 *
 * This function is used internally to write data to the acceleromter via RP2040 I2C hardware interface
 * \param target_addr I2C address command is directed at
 * \param register_addr I2C register address to start writing at
 * \param length of write (MAX I2C_BUF_SIZE)
 *
 * \returns read byte length if successful (value > 0) otherwise status code (value < 0) 
 */

int es100_i2c_write_register_blocking_uint8(uint8_t const target_addr, uint8_t const register_addr, uint8_t length);

/*!
 * RP2040 specific I2C convenience function that returns a single byte
 * 
 * This function is used internally to read data a single byte at a type via RP2040 I2C Hardware interface
 *
 * \param  register_addr I2C register address to read
 *
 * \returns requested byte value if successful (value >= 0) otherwise status code (value < 0)
 */

int es100_i2c_return_register_byte(uint8_t const register_addr);


/*!
 * RP2040 specific I2C convenience function that sets a single bit
 *
 * This function is used internally to set a single bit to a particular value (0 or 1) via RP2040 I2C hardware interface.
 * Will check value before writing, if value is already set to mode then function will not bother writing it.
 *
 * \param mode value to set (0 or 1)
 * \param register_address I2C register address to write
 * \param bit_position position within the byte to modify (7-0)
 *
 * \returns mode if successful (value == 0 or 1) otherwise status code (value < 0)
 */

int es100_i2c_set_register_bit_mode(bool mode, uint8_t register_address, uint8_t bit_position);
#endif
