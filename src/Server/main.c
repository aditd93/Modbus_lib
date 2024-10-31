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
    Modbus_Server *MBS = NULL; // First pointer to one Server
    while(true) {
        printf("\n------------------------\n");
        printf("Main Menu:\n"
        "   1.  Create new Modbus Server\n"
        "   2.  Modify Registers\n"
        "   3.  Print DataBank\n"
        "   4.  Listen to incoming connections\n"
        "   5.  Close Server & Exit\n"
        );
        printf("------------------------\n");
        int option;
        printf("Enter your choose: \n");
        scanf("%d",&option);
        switch(option) {
            case 1:
            // Create new Modbus Server
                printf("\n========================\n");
                printf("\nCreate a new Modbus Server:\n");
                // create new Modbus Server with new DataBank
                int coils, discrete_inputs, inputs_registers, hoilding_registers = 0;
                printf("Enter num of coils:\n");                scanf("%d",&coils);
                printf("Enter num of discrete inputs:\n");      scanf("%d",&discrete_inputs);
                printf("Enter num of inputs registers:\n");     scanf("%d",&inputs_registers);
                printf("Enter num of hoilding_registers:\n");   scanf("%d",&hoilding_registers);
                MBS = create_server(port, server_ip, coils,discrete_inputs,inputs_registers,hoilding_registers); 
            break;

            case 2:
            // Modify databank registers
                if(MBS != NULL) {
                    printf("\n========================\n");
                    modify_registers(MBS);
                }
                else {
                    printf("Modbus Server hasn't created yet\n");
                }
                
            break;

            case 3:
            // Print modbus server's databank
                if (MBS != NULL) {
                    print_databank(MBS);
                }
                else {
                    printf("Modbus Server hasn't created yet\n");
                }
               
            break;

            case 4:
            // Open server's listening socket for incoming TCP connections
                if (MBS != NULL) {
                    printf("Listen to new connection on port: %d\n", port);
                    server_listen(MBS);
                    connect_server(MBS);
                }
                else {
                    printf("Modbus Server hasn't created yet\n");
                }
                break;
            case 5:
                close_server(MBS);
                return 0;

            default:
                printf("Please enter a choice between 1-4\n");
            break;
        }
    }

    return 0;
}