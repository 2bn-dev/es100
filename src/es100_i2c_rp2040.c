/*! \file
 * \brief Code for RP2040 specific I2C implementation
 */

#include "es100_i2c_rp2040.h"
#include "es100_debug.h"


void es100_i2c_init_local(){
	_DBG("i2c_init()");
	i2c_init(I2C_INTERFACE, I2C_SPEED);
	gpio_set_function(PIN_I2C_SDA, GPIO_FUNC_I2C);
	gpio_set_function(PIN_I2C_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(PIN_I2C_SDA);
	gpio_pull_up(PIN_I2C_SCL);

	//Tell PICOTOOL about I2C pins
	_DBG("i2c_init() complete");
}

int es100_i2c_read_register_blocking_uint8(uint8_t const target_addr, uint8_t const register_addr, uint8_t length){
	_DBG("blocking register read target_addr: 0x%02x, register_addr: 0x%02x, rbxuf size: %d", target_addr, register_addr, length);

	if(length > I2C_BUF_SIZE)
		return I2C_RXBUF_TOO_SMALL;

	//Reserved I2C addresses
	if((target_addr&0x78) == 0 || (target_addr & 0x78) == 0x78) 
		return I2C_INVALID_TARGET_ADDRESS;
	
	if(!es100_valid_read_address(register_addr)) 
		return I2C_INVALID_REGISTER_ADDRESS;

	int ret = 0;
    _DBG("WRITE addr(0x%02x) register_addr(0x%02x), size(0x%02x)", target_addr, register_addr, sizeof(register_addr));
	ret = i2c_write_blocking_until(I2C_INTERFACE, target_addr, &register_addr, sizeof(register_addr), false, make_timeout_time_ms(I2C_WRITE_TIMEOUT_MS));
	if(ret == PICO_ERROR_TIMEOUT) return I2C_WRITE_TIMEOUT;
	if(ret == PICO_ERROR_GENERIC) return I2C_ADDR_DIDNT_ACK;
	if(ret < 1) return I2C_ERROR_UNKNOWN;

	memset(es100_i2c_get_rxbuf_ptr(), 0x0, I2C_BUF_SIZE);
    _DBG("READ addr(0x%02x) length(0x%02x)", target_addr, length);

	ret = i2c_read_blocking_until(I2C_INTERFACE, target_addr, es100_i2c_get_rxbuf_ptr(), length, false, make_timeout_time_ms(I2C_READ_TIMEOUT_MS));
	if(ret == PICO_ERROR_TIMEOUT) return I2C_READ_TIMEOUT;
	if(ret == PICO_ERROR_GENERIC) return I2C_ADDR_DIDNT_ACK;
	if(ret < 1) return I2C_ERROR_UNKNOWN;
	
	_DBG("Blocking read result: target_addr: 0x%02x, register_addr: 0x%02x, rbxuf size: %d, rxbuf: 0x%02x%02x%02x%02x%02x%02x%02x%02x ret: %d", target_addr, register_addr, length, es100_i2c_get_rxbuf_ptr()[0], es100_i2c_get_rxbuf_ptr()[1], es100_i2c_get_rxbuf_ptr()[2], es100_i2c_get_rxbuf_ptr()[3], es100_i2c_get_rxbuf_ptr()[4], es100_i2c_get_rxbuf_ptr()[5], es100_i2c_get_rxbuf_ptr()[6], es100_i2c_get_rxbuf_ptr()[7], ret);
	return ret; // Number of bytes read
}

int es100_i2c_return_register_byte(uint8_t const register_addr){
	uint8_t length = 1;
	int ret = es100_i2c_read_register_blocking_uint8(es100_get_i2c_target_address(), register_addr, length);
	_DBG("single byte read: %02x, result: %02x", register_addr, es100_i2c_get_rxbuf_ptr()[0]);

	if(ret < 1) 
		return ret;
	int result = 0;
	result = es100_i2c_get_rxbuf_ptr()[0];
	return result;
}	

int es100_i2c_write_register_blocking_uint8(uint8_t const target_addr, uint8_t const register_addr, uint8_t length){
	if(length > I2C_BUF_SIZE)
		return I2C_RXBUF_TOO_SMALL;
	_DBG("blocking register write target_addr: %02x, register_addr: %02x, tbxuf size: %d", target_addr, register_addr, length);

	//Reserved I2C addresses 
        if((target_addr&0x78) == 0 || (target_addr & 0x78) == 0x78){
                return I2C_INVALID_TARGET_ADDRESS;
        }

	if(!es100_valid_write_address(register_addr)){
                return I2C_INVALID_REGISTER_ADDRESS;
        }

	int ret = 0;
	for(int i = I2C_BUF_SIZE; i >= 0; --i){
		es100_i2c_get_txbuf_ptr()[i] = es100_i2c_get_txbuf_ptr()[i-1];
	}
	es100_i2c_get_txbuf_ptr()[0] = register_addr;

	ret = i2c_write_blocking_until(I2C_INTERFACE, target_addr, es100_i2c_get_txbuf_ptr(), length+1, false, make_timeout_time_ms(I2C_WRITE_TIMEOUT_MS));
	memset(es100_i2c_get_txbuf_ptr(), 0x0, I2C_BUF_SIZE);
	if(ret == PICO_ERROR_TIMEOUT)
                return I2C_WRITE_TIMEOUT;
        if(ret == PICO_ERROR_GENERIC)
                return I2C_ADDR_DIDNT_ACK;
        if(ret < 1)
                return I2C_ERROR_UNKNOWN;
        

	return ret;
}

int es100_i2c_set_register_bit_mode(bool mode, uint8_t register_address, uint8_t bit_position){
	int ret;
	uint8_t length = 1;
	
	ret = es100_i2c_read_register_blocking_uint8(es100_get_i2c_target_address(), register_address, length);
	if( ret < 1){
		return ret;
	}

	uint8_t current_mode = es100_get_bit(es100_i2c_get_rxbuf_ptr()[0], bit_position);

	_DBG("Address: %02x, Position: %d, rxbuf: %02x, current_mode: %d desired mode: %d ", register_address, bit_position,  es100_i2c_get_rxbuf_ptr()[0], current_mode, mode);
	if(current_mode == mode){
		return current_mode;
	}
	es100_i2c_get_txbuf_ptr()[0] = es100_flip_bit(es100_i2c_get_rxbuf_ptr()[0], bit_position);
	_DBG("Address: %02x, Position: %d, txbuf: %02x", register_address, bit_position, es100_i2c_get_txbuf_ptr()[0]);
	ret = es100_i2c_write_register_blocking_uint8(es100_get_i2c_target_address(), register_address, length);
	if(ret < 1){
		_DBG("WRITE ERROR %d", ret);
		return ret;
	}

	return mode;
}

