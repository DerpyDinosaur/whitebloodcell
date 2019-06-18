CC = gcc
OBJ = tools.o scan.o
EXEC = whitebc
FLAGS = -g -Wall -lpthread -lssl -lcrypto -ljson-c
RM = rm

all: $(EXEC)

$(EXEC):$(OBJ) whitebc.o
	$(CC) $(FLAGS) $(OBJ) whitebc.o -o $(EXEC)

whitebc.o: whitebc.c tools.h scan.h
	$(CC) $(FLAGS) -c whitebc.c

tools.o: tools.c tools.h scan.h
	$(CC) $(FLAGS) -c tools.c

scan.o: scan.c scan.h
	$(CC) $(FLAGS) -c scan.c

clean:
	$(RM) $(OBJ) whitebc.o $(EXEC) 