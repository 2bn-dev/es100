/*! \file
 * \brief Contains non-hardware specific I2C code
 */

#include "es100_i2c.h"
#include "es100_registers.h"
#include "es100_debug.h"

uint8_t* rxbuf = 0; //!< I2C receive byte buffer
uint8_t* txbuf = 0; //!< I2C transmit byte buffer


uint8_t es100_i2c_target_address = ES100_I2C_DEFAULT_ADDRESS;


bool es100_get_bit(uint8_t const value, uint8_t const bit_number){ 
        return (value&(1<<bit_number)) >> bit_number;
}
uint8_t es100_flip_bit(uint8_t const value, uint8_t const bit_number){ 
	return value ^ (1 << bit_number);
}


uint8_t es100_get_i2c_target_address(){
	return es100_i2c_target_address;
}

int es100_set_i2c_target_address(uint8_t target_address){
	//Reserved I2C addresses
	if((target_address&0x78) == 0 || (target_address & 0x78) == 0x78)
		return I2C_INVALID_TARGET_ADDRESS;
	es100_i2c_target_address = target_address;
	
	return I2C_SUCCESS;
}

uint8_t* es100_i2c_get_rxbuf_ptr(){
	if(rxbuf == 0){
	        rxbuf = (uint8_t*) malloc(I2C_BUF_SIZE*sizeof(uint8_t));
	}
	return txbuf;
}
uint8_t* es100_i2c_get_txbuf_ptr(){
	if(txbuf == 0){
	        txbuf = (uint8_t*) malloc(I2C_BUF_SIZE*sizeof(uint8_t));
	}
	return txbuf;
}

