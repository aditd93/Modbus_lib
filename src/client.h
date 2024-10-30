#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
// Variables and Macros
#define READ_COILS 0x01
#define READ_DISCRETE_INPUTS 0x02
#define READ_MULTIPLE_HOLDING_REGISTERS 0x03
#define READ_INPUT_REGISTERS 0X04
#define WRITE_SINGLE_COIL 0x05
#define WRITE_SINGLE_HOLDING_REGISTER 0X06
#define BUFFER 256
#define SERVER_PORT 502

typedef struct Modbus_Server {
    char* IP; // Server's IP
    int port; // Server port=502 default
} Modbus_Server;

typedef struct Modbus_Client {
    
} Modbus_Client;



/*  
    Notes:

*/

// functions


#endif /* CLIENT_HEADER_H_ */