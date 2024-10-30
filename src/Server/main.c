#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "server.h"
#include "../ADU.h"

/*
    This main.c file for server execution
*/

int main(int argc, char **argv) {
    int opt;
    char *server_ip = NULL; int port = SERVER_PORT;
    while((opt = getopt(argc,argv,"a:p:")) != -1) { // get user inputs
        switch(opt) {
            case 'a':
                server_ip = optarg;
                break;
            case 'p':
                // Default port for Modbus TCP is 502
                port = atoi(optarg);
                if(port < 1 || port > 65535){
                    fprintf(stderr, "Invailid port number: %d. Valid Range [1, 65535]\n", port);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
            default:
                fprintf(stderr, "Usage: %s [-a server_ip] [-p port]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Menu for server
    Modbus_Server *MBS = create_server(port,server_ip,2,2,2,2); // ModBus Server 
    MBS->databank->input_registers[0] = 25;
    MBS->databank->input_registers[1] = 16;
    uint16_t buffer[2];
    buffer[0] = MBS->databank->input_registers[0];
    buffer[1] = MBS->databank->input_registers[1];
    printf("First input_reg: %d\tSecond input_reg:%d\n",MBS->databank->input_registers[0],MBS->databank->input_registers[1]);
    server_listen(MBS);
    connect_server(MBS);
    size_t bytes2send = sizeof(buffer);
    size_t bytes = send(MBS->socket,buffer,bytes2send,0);
    if(bytes < 0) {
        perror("Failed to send input_regs to client\n");
    }
    printf("%ld bytes were sent to client\n", bytes);
    close_server(MBS);
    return 0;
}