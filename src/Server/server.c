#include "server.h"
#include "../ADU.h"

/*
   |--------| Request      ========>   Indication |--------|
   | Client |                                     | Server |
   |--------| Confirmation <========      Respond |--------|
*/

DataBank *create_DB(int num_coils, int num_discrete_inputs, int num_input_registers, int num_holding_registers) {
    DataBank *new_DB = (DataBank*)malloc(sizeof(DataBank));
    new_DB->coils = calloc(sizeof(bool),num_coils);
    new_DB->discrete_inputs = calloc(sizeof(bool),num_discrete_inputs);
    new_DB->input_registers = calloc(sizeof(uint16_t),num_input_registers);
    new_DB->holding_registers = calloc(sizeof(uint16_t),num_holding_registers);
    if(new_DB == NULL || new_DB->coils == NULL || new_DB->discrete_inputs == NULL || new_DB->input_registers == NULL || new_DB->holding_registers == NULL) {
        perror("Memory allocation failed to create new DataBank...\n");
        exit(0);
    }
    new_DB->coil_length = num_coils;    new_DB->discrete_length = num_discrete_inputs;
    new_DB->inputs_length = num_input_registers;    new_DB->holding_length = num_holding_registers;
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
    printf("Server is closed\n");
    // check for failes while closing will be added later
    DataBank *DB = mbs->databank;
    free(DB->coils);free(DB->discrete_inputs); // free coild and discrete inputs
    free(DB->input_registers);free(DB->holding_registers); // free input and holding registers
    printf("Server's data bank erased\n");
}

size_t server_indication() {
    /*
        When server receives a request from client, it must check the ADU validity.
        Finish the validation check, and then determine next step - whether a client wants to read or write data.
    */

}

void modify_registers(Modbus_Server *mbs) {
    // Change the registers manually, real-life systems modify according to a physical process
    while (true) {
        int option;
        printf("Enter which registers group you would like to modify:\n"
        "1. Coils\n"
        "2. Discrete Inputs\n"
        "3. Input registers\n"
        "4. Holding registers\n"
        "5. Back to main menu\n"
        );
        scanf("%d",&option);
        switch(option) {
            int index;
            case 1:
            // Coils
                if(mbs->databank->coil_length >0) {
                    bool new_coil_value; int temp;
                    printf("Enter coil's index you would like to modify: 0 to %d\n",mbs->databank->coil_length - 1);
                    scanf("%d",&index);
                    printf("Current value is: %d\n"
                    "Enter new value:\n", mbs->databank->coils[index]);
                    scanf("%d",&temp);
                    new_coil_value = (temp == 1 ? true:false);
                    mbs->databank->coils[index] = new_coil_value;
                    printf("Coil has been updated successfully\n") ;
                }
                else {
                    printf("No such index! \n");
                }
            break;

            case 2:
            // Discrete inputs
                if(mbs->databank->discrete_inputs >0) {
                    bool new_discrete_value; int temp = 0;
                    printf("Enter discrete's index you would like to modify: 0 to %d\n",mbs->databank->discrete_length - 1);
                    scanf("%d",&index);
                    printf("Current value is: %d\n"
                    "Enter new value:\n", mbs->databank->discrete_inputs[index]);
                    scanf("%d",&temp);
                    new_discrete_value = (temp == 1 ? true:false);
                    mbs->databank->discrete_inputs[index] = new_discrete_value;
                    printf("Discrete input has been updated successfully\n") ;
                }
                else {
                    printf("No such index! \n");
                }
            break;

            case 3:
            // Input registers
                if(mbs->databank->inputs_length >0) {
                    uint16_t new_input_value;
                    printf("Enter input register's index you would like to modify: 0 to %d\n",mbs->databank->inputs_length - 1);
                    scanf("%d",&index);
                    printf("Current value is: %d\n"
                    "Enter new value:\n", mbs->databank->input_registers[index]);
                    scanf("%hd",&new_input_value);
                    mbs->databank->input_registers[index] = new_input_value;
                    printf("Input register has been updated successfully\n") ;
                }
                else {
                    printf("No such index! \n");
                }
            break;

            case 4:
            // Holding registers
                if(mbs->databank->holding_length >0) {
                    uint16_t new_holding_value;
                    printf("Enter holding register's index you would like to modify: 0 to %d\n",mbs->databank->holding_length - 1);
                    scanf("%d",&index);
                    printf("Current value is: %d\n"
                    "Enter new value:\n", mbs->databank->holding_registers[index]);
                    scanf("%hd",&new_holding_value);
                    mbs->databank->holding_registers[index] = new_holding_value;
                    printf("Holding register has been updated successfully\n") ;
                }
                else {
                    printf("No such index! \n");
                }
            break;
            case 5:
                printf("Go back to main menu\n");
                return;
            default:
                printf("Please enter a valid option [1-4]");
            break;
        }
        return;
    }
}

void print_databank(Modbus_Server *mbs) {
    // print the amount of registers and their values
    printf("Server's Databank current registers:\n");

    if(mbs->databank->coil_length > 0) {
        printf("\n====================================\n");
        printf("Total Coils:%d\n", mbs->databank->coil_length);
        printf("[");
        for(int i=0; i<mbs->databank->coil_length; i++) {
            mbs->databank->coils[i] == true ? printf("true, ") : printf("false, ");
        }
        printf("]\n");
    }

    if(mbs->databank->discrete_length > 0) {
        printf("\n====================================\n");
        printf("Total Discrete registers:%d\n", mbs->databank->discrete_length);
        printf("[");
        for(int i=0; i<mbs->databank->discrete_length; i++) {
            mbs->databank->discrete_inputs[i] == true ? printf("true, ") : printf("false, ");
        }
        printf("]\n");
    }

    if(mbs->databank->inputs_length > 0) {
        printf("\n====================================\n");
        printf("Total Input registers:%d\n", mbs->databank->inputs_length);
        printf("[");
        for(int i=0; i<mbs->databank->inputs_length; i++) {
            printf("%d, ", mbs->databank->input_registers[i]);
        }
        printf("]\n");
    }

    if(mbs->databank->holding_length > 0) {
        printf("\n====================================\n");
        printf("Total Holding registers:%d\n", mbs->databank->holding_length);
        printf("[");
        for(int i=0; i<mbs->databank->holding_length; i++) {
            printf("%d, ", mbs->databank->holding_registers[i]);
        }
        printf("]\n");
    }
    
}
