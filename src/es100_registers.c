/*! \file
 * \brief Code for validating register read and write addresses
 */

#include "es100_registers.h"

uint8_t ES100_VALID_READ_ADDRESSES[] = {
    ES100_CONTROL_0_REG,      
    ES100_CONTROL_1_REG,      
    ES100_IRQ_STATUS_REG,    
    ES100_STATUS0_REG,       
    ES100_YEAR_REG,          
    ES100_MONTH_REG,         
    ES100_DAY_REG,           
    ES100_HOUR_REG,          
    ES100_MINUTE_REG,
    ES100_SECOND_REG,
    ES100_NEXT_DST_MONTH_REG,
    ES100_NEXT_DST_DAY_REG,
    ES100_NEXT_DST_HOUR_REG,
    ES100_DEVICE_ID_REG
};//!< Array of valid read register addresses

uint8_t ES100_VALID_WRITE_ADDRESSES[] = {
    ES100_CONTROL_0_REG,
    ES100_CONTROL_1_REG


};//!< Array of valid write register addresses

bool es100_valid_read_address(uint8_t address){
	bool register_found = false;
        for(uint8_t i = 0; i < sizeof(ES100_VALID_READ_ADDRESSES); i++){
                if(ES100_VALID_READ_ADDRESSES[i] == address){
                        register_found = true;
                        break;
                }
        }

	return register_found;
}


bool es100_valid_write_address(uint8_t address){
	bool register_found = false;
        for(uint8_t i = 0; i < sizeof(ES100_VALID_READ_ADDRESSES); i++){
                if(ES100_VALID_READ_ADDRESSES[i] == address){
                        register_found = true;
                        break;
                }
        }

        return register_found;
}

