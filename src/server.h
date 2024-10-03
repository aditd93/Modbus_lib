#ifndef SERVER_H_
#define SERVER_H_

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

typedef struct DataBank {
    bool *coils;
    bool *discrete_inputs;
    uint16_t *input_registers;
    uint16_t *holding_registers;
} DataBank;

typedef struct Modbus_Server {
    DataBank *databank;
    char* IP; // Server's IP
    int port; // Server port
    int fd; // file descriptor
    int socket;
    struct sockaddr_in address; // socket, port, ip

} Modbus_Server;



/*  
    Notes:

*/

// functions
DataBank *create_DB(int num_coils, int num_discrete_inputs, int num_input_registers, int num_holding_registers);
Modbus_Server *create_server(int port, char *server_ip, int num_coils, int num_discrete_inputs, int num_input_registers, int num_holding_registers);
void server_listen(Modbus_Server* mbs);


#endif /* SERVER_HEADER_H_ */