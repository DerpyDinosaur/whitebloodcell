CC = gcc
OBJ = tools.o scan.o web.o feed.o
HEAD = tools.h scan.h web.h feed.h
EXEC = whitebc
FLAGS = -g -Wall -lpthread -lssl -lcrypto -lcurl
RM = rm

all: $(EXEC)

$(EXEC):$(OBJ) whitebc.o
	$(CC) $(FLAGS) $(OBJ) whitebc.o -o $(EXEC)

whitebc.o: whitebc.c $(HEAD)
	$(CC) $(FLAGS) -c whitebc.c

tools.o: tools.c tools.h
	$(CC) $(FLAGS) -c tools.c

feed.o: feed.c tools.h
	$(CC) $(FLAGS) -c feed.c

scan.o: scan.c scan.h tools.h feed.h
	$(CC) $(FLAGS) -c scan.c

web.o: web.c tools.h
	$(CC) $(FLAGS) -c web.c

clean:
	$(RM) $(OBJ) whitebc.o $(EXEC) 