#include "server.h"
#include "../ADU.h"

/*
   |--------| Request      ========>   Indication |--------|
   | Client |                                     | Server |
   |--------| Confirmation <========      Respond |--------|
*/

DataBank *create_DB(int num_coils, int num_discrete_inputs, int num_input_registers, int num_holding_registers) {
    DataBank *new_DB = (DataBank*)malloc(sizeof(DataBank));
    new_DB->coils = (bool*)malloc(sizeof(bool) * num_coils);
    new_DB->discrete_inputs = (bool*)malloc(sizeof(bool) * num_discrete_inputs);
    new_DB->input_registers = (uint16_t*)malloc(sizeof(uint16_t) * num_input_registers);
    new_DB->holding_registers = (uint16_t*)malloc(sizeof(uint16_t) * num_holding_registers);
    if(new_DB == NULL || new_DB->coils == NULL || new_DB->discrete_inputs == NULL || new_DB->input_registers == NULL || new_DB->holding_registers == NULL) {
        perror("Memory allocation failed to create new DataBank...\n");
        exit(0);
    }
    return new_DB;
}

Modbus_Server *create_server(int port, char *server_ip, int num_coils, int num_discrete_inputs, int num_input_registers, int num_holding_registers) {
    Modbus_Server *new_mbs = (Modbus_Server*)malloc(sizeof(Modbus_Server));
    if(new_mbs == NULL) {
        perror("Memory allocation failed to create new Modbus Server...\n");
        exit(EXIT_FAILURE);
    }
    new_mbs->databank = create_DB(num_coils, num_discrete_inputs, num_input_registers,num_holding_registers);

    // configure the server
    // Create server's socket file descriptor
    if((new_mbs->fd= socket(AF_INET, SOCK_STREAM, 0) ) < 0) {
        perror("Create socket failed\n"); // In case of a fail
        exit(EXIT_FAILURE);
    }

    // Bind the socket
    new_mbs->address.sin_family = AF_INET;
    new_mbs->address.sin_addr.s_addr = htonl(INADDR_ANY);
    new_mbs->address.sin_port = htons(port);
    int reuse_sock = 1; // free port when finishes program
    // setsockopt(new_mbs->fd,SOL_SOCKET,SO_REUSEADDR,(char*)&reuse_sock,sizeof(reuse_sock));
    if(bind(new_mbs->fd, (struct sockaddr*)&new_mbs->address, sizeof(new_mbs->address)) < 0) {
        perror("Bind failed, exit program\n");
        exit(EXIT_FAILURE);
    }

    return new_mbs;
}

void server_listen(Modbus_Server *mbs) {
    // listen to client request to establish a TCP connection
    if(listen(mbs->fd,3) < 0) {
        perror("listen to incoming connections requests failed, exit program\n");
        exit(EXIT_FAILURE);
    }
    printf("Listenning to incoming TCP requests...\n");
}

int connect_server(Modbus_Server *mbs) {

    socklen_t addrlen = sizeof(mbs->address);
    // Accept client connection and create a control socket 
    if((mbs->socket = accept(mbs->fd, (struct sockaddr*)&mbs->address, &addrlen)) < 0) {
        perror("Accept new connection failed, exit program\n");
        return -1;
    }
    else{
        printf("Connection is established\n");
        return 1;
    }
}

void close_server(Modbus_Server *mbs) {
    // close all server's componenets
    close(mbs->socket);
    close(mbs->fd);

    // check for failes while closing will be added later
}

size_t server_indication() {
    /*
        When server receives a request from client, it must check the ADU validity.
        Finish the validation check, and then determine next step - whether a client wants to read or write data.
    */

}
