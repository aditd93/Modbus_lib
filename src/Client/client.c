#include "client.h"
#include "../ADU.h"

/*
   |--------| Request      ========>   Indication |--------|
   | Client |                                     | Server |
   |--------| Confirmation <========      Respond |--------|
*/

Modbus_Server *create_server(char *server_ip, int port) {
    Modbus_Server *new_server = (Modbus_Server*)malloc(sizeof(Modbus_Server));
    new_server->IP = server_ip;
    new_server->port = port;
    return new_server;
}

void connect_to_server(Modbus_Server *MBS) {
    int new_socket = 0; struct sockaddr_in addr;
    // Create file descriptor
    if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Failed create file descriptor, exit program\n");
        exit(EXIT_FAILURE);
    }
    // Set the server adderss
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MBS->port);
    if (inet_pton(AF_INET, MBS->IP, &addr.sin_addr) <= 0) {
        perror("Invalid address, exit program\n");
        close(new_socket);
        exit(EXIT_FAILURE);
    }
    if (connect(new_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Connection failed, exit program\n");
        close(new_socket);
        exit(EXIT_FAILURE);
    }
    MBS->socket = new_socket;
}