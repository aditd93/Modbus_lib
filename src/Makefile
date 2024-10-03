CC = gcc
CFLAGS = -Wall -Wextra -lpthread

SRC = main.c server.c
HDR = server.h
OBJ = $(SRC:.c=.o)
TARGET = ModbusLib

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)