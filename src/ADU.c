#include "ADU.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>

ADU *request_read(int function_code, uint16_t target_address, uint16_t quantity) { 
    /* 
        read request initiated by Client. Reading requests need a target address of the desired data registers and the range of the addresses to receive.
        All addresses ranges are the same for all registers 0-65535
        Function codes:
        READ_COIL 0x01
        READ_DISCRETE_INPUTS 0x02
        READ_MULTIPLE_HOLDING_REGISTERS 0x03
        READ_INPUT_REGISTERS 0X04
    */
    // initiated by Client
    // None-PDU fields
    ADU *adu_req = (ADU*)malloc(sizeof(ADU));
    PDU *pdu_req = (PDU*)malloc(sizeof(PDU));
    if(adu_req == NULL || pdu_req == NULL) {
        perror("failed allocate new ADU/PDU Request, exit program\n");
        exit(EXIT_FAILURE);
    }
    adu_req->Transaction_ID = 1;
    adu_req->Protocol_ID = 0x0; // constant
    adu_req->Unit_ID = 0xFF;    // constant

    // prepare the data to be read from server
    adu_req->pdu = adu_req;
    adu_req->pdu->function_code = function_code; // read coil/discrete/holding/input registers
    *(uint16_t*)adu_req->pdu->data = (uint16_t*)malloc(sizeof(uint16_t)*2);
    *(uint16_t*)adu_req->pdu->data = target_address;    // starting address
    *(uint16_t*)(adu_req->pdu->data+1) = quantity;  // num of registers to be read

    return adu_req;
}

ADU *request_write_bit(int function_code, uint16_t target_address, bool new_value) {
    // initiated by Client
    // None-PDU fields
    ADU *adu_req = (ADU*)malloc(sizeof(ADU));
    if(adu_req == NULL) {
        perror("failed allocate new ADU Request, exit program\n");
        exit(EXIT_FAILURE);
    }
    adu_req->Transaction_ID = 1;
    adu_req->Protocol_ID = 0x0;
    adu_req->Unit_ID = 0xFF;

    adu_req->pdu->function_code = function_code; // read / write registers


}

ADU *request_write_word(int function_code, uint16_t target_address, uint16_t new_value) {
    // initiated by Client
    // None-PDU fields
    ADU *adu_req = (ADU*)malloc(sizeof(ADU));
    if(adu_req == NULL) {
        perror("failed allocate new ADU Request, exit program\n");
        exit(EXIT_FAILURE);
    }
    adu_req->Transaction_ID = 1;
    adu_req->Protocol_ID = 0x0;
    adu_req->Unit_ID = 0xFF;

    adu_req->pdu->function_code = function_code; // read / write registers


}

ADU *response_read(int function_code, uint16_t target_address, uint16_t quantity, DataBank *databank) {
    /* 
        read request initiated by Client. Reading requests need a target address of the desired data registers and the range of the addresses to receive.
        All addresses ranges are the same for all registers 0-65536
        Function codes:
        READ_COIL 0x01
        READ_DISCRETE_INPUTS 0x02
        READ_MULTIPLE_HOLDING_REGISTERS 0x03
        READ_INPUT_REGISTERS 0X04
    */
    // initiated by Client
    // None-PDU fields
    ADU *adu_res = (ADU*)malloc(sizeof(ADU));
    PDU *pdu_res = (PDU*)malloc(sizeof(PDU));
    if(adu_res == NULL || pdu_res == NULL) {
        perror("failed allocate new ADU/PDU Response, exit program\n");
        exit(EXIT_FAILURE);
    }
    adu_res->Transaction_ID = 1;
    adu_res->Protocol_ID = 0x0; // constant
    adu_res->Unit_ID = 0xFF;    // constant
    adu_res->pdu = pdu_res;
    switch(function_code) {
        case READ_COILS:
            // reading bits
            *(bool*)adu_res->pdu->data = (bool*)malloc(sizeof(bool)* quantity);
            for (int i =0; i<quantity; i++) {
                // copy coils from databank to respond pdu
                *(bool*)(pdu_res->data+i) = databank->coils[target_address+i];
            }
            break;
        case READ_DISCRETE_INPUTS:
            // reading bits
            *(bool*)adu_res->pdu->data = (bool*)malloc(sizeof(bool)* quantity);
            for (int i =0; i<quantity; i++) {
                // copy coils from databank to respond pdu
                *(bool*)(pdu_res->data+i) = databank->discrete_inputs[target_address+i];
            }
            break;
        case READ_INPUT_REGISTERS:
            // reading words
            *(uint16_t*)adu_res->pdu->data = (uint16_t*)malloc(sizeof(uint16_t)* quantity);
            for (int i =0; i<quantity; i++) {
                // copy coils from databank to respond pdu
                *(uint16_t*)(pdu_res->data+i) = databank->input_registers[target_address+i];
            }
            break;
        case READ_MULTIPLE_HOLDING_REGISTERS:
            // reading words
            *(uint16_t*)adu_res->pdu->data = (uint16_t*)malloc(sizeof(uint16_t)* quantity);
            for (int i =0; i<quantity; i++) {
                // copy coils from databank to respond pdu
                *(uint16_t*)(pdu_res->data+i) = databank->holding_registers[target_address+i];
            }
            break;
    }
    return adu_res;
}