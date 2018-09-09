PROGRAMS := servidor cliente
SOURCES := function
CC := gcc
FLAGS := -Wall -Werror -pedantic -Wextra -fno-stack-protector -std=c11

all: $(SOURCES) $(PROGRAMS) rmobj

function: function.h function.c
	$(CC) $(FLAGS) -c function.c
servidor: servidor.c function.o
	$(CC) $(FLAGS) servidor.c function.o -o servidorFTP 
cliente: cliente.c function.o
	$(CC) $(FLAGS) cliente.c function.o -o clienteFTP

rmobj: function.o
	rm -rf *.o

clean: 
	rm -rf clienteFTP servidorFTP
