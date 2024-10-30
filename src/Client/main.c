#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "client.h"
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

    // Menu for client

    Modbus_Server *MBS1 = create_server(server_ip,port);
    connect_to_server(MBS1);
    printf("Client is connected to Server\n");
    uint16_t buffer[3] = {0, 1};
    size_t bytes = recv(MBS1->socket,buffer,sizeof(buffer),0); // receive first coil from server
    printf("First input_reg: %d\nSecond input_reg:%d\nbytes: %ld\n",buffer[0],buffer[1],bytes);
    close(MBS1->socket);
    return 0;

}