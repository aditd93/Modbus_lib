#ifndef ADU_H_
#define ADU_H_
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

// Variables and Macros
#define READ_COILS 0x01
#define READ_DISCRETE_INPUTS 0x02
#define READ_MULTIPLE_HOLDING_REGISTERS 0x03
#define READ_INPUT_REGISTERS 0X04
#define WRITE_SINGLE_COIL 0x05
#define WRITE_SINGLE_HOLDING_REGISTER 0X06


typedef struct PDU {
    uint8_t function_code;
    void* data;
} PDU;

// MBAP header 
typedef struct ADU {
    uint16_t Transaction_ID; // 
    uint16_t Protocol_ID; // =0x00 Modbus protocol
    uint16_t length;    //
    uint8_t Unit_ID; // =0xFF
    PDU *pdu;

} ADU;


/*  
    Notes:

*/

// functions
ADU *request_read(int function_code, uint16_t target_address, uint16_t quantity);
ADU *request_write_bit(int function_code, uint16_t target_address, bool new_value);
ADU *request_write_word(int function_code, uint16_t target_address, uint16_t new_value);
ADU *response();


#endif /* ADU_HEADER_H_ */