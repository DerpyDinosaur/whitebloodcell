CC = gcc
# OBJ = 
EXEC = whitebloodcell
CFLAGS = -g -Wall -lpthread -lssl -lcrypto
RM = rm

all: $(EXEC)

$(EXEC):$(OBJ) whitebloodcell.o
	$(CC) $(CFLAGS) $(OBJ) whitebloodcell.o -o $(EXEC)

whitebloodcell.o: whitebloodcell.c
	$(CC) $(CFLAGS) -c whitebloodcell.c

clean:
	$(RM) $(OBJ) whitebloodcell.o $(EXEC) 