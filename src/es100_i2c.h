/*! \file
 * \brief Contains non-hardware specific I2C headers
 */

#ifndef ES100_I2C_H
#define ES100_I2C_H

#include <inttypes.h>
#include <stdbool.h>

#define ES100_I2C_DEFAULT_ADDRESS 0x32


#define I2C_BUF_SIZE 9

enum I2C_STATUS_CODE {
        I2C_SUCCESS                     = 0,
        I2C_INVALID_TARGET_ADDRESS      = -1,
        I2C_INVALID_REGISTER_ADDRESS    = -2,
        I2C_RXBUF_TOO_SMALL             = -3,
        I2C_WRITE_TIMEOUT               = -4,
        I2C_READ_TIMEOUT                = -5,
        I2C_ADDR_DIDNT_ACK              = -6,
        I2C_ERROR_UNKNOWN               = -7
};

typedef enum I2C_STATUS_CODE i2c_status_code_t;

/*! Get current configured I2C target address
 * \returns uint8_t Current I2C target address
 */

uint8_t es100_get_i2c_target_address();

/*! Set current configured I2C target address
 *  \param target_address Target I2C address
 *  \returns Status code I2C_SUCCESS or I2C_INVALID_TARGET_ADDRESS (for reserved I2C addresses)
 */
int es100_set_i2c_target_address(uint8_t const target_address);

/*! Convience function for getting a single bit from a byte
 * \param value Byte value to access
 * \param bit_number Bit position to return (7-0)
 * \returns bool 0 or 1
 */
bool es100_get_bit(uint8_t const value, uint8_t const bit_number);

/*! Convenience function for flipping a single bit in a byte
 * \param value Byte value to modify
 * \param bit_number bit position to flip
 * \returns uint8_t modified byte value
 */
uint8_t es100_flip_bit(uint8_t const value, uint8_t const bit_number);

/*! Internal function used to malloc and get the txbuf ptr
 * \ returns uint8_t pointer to txbuf
 */
uint8_t* es100_i2c_get_txbuf_ptr();

/*! Internal function used to malloc and get the rxbuf ptr
 * \returns uint8_t pointer to rxbuf
 */
uint8_t* es100_i2c_get_rxbuf_ptr();

#endif
