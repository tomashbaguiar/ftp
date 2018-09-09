CC = gcc
FLAGS = -Wall -Werror -pedantic -std=c11 -Wextra

all: function cliente servidor clean

function: function.h function.c
	$(CC) $(FLAGS) -c function.c

cliente: cliente.c function.o
	$(CC) $(FLAGS) cliente.c function.o -o clienteFTP

servidor: servidor.c function.o
		$(CC) $(FLAGS) servidor.c function.o -o servidorFTP

clean: function.o
	rm function.o

PROGRAMS := function servidor cliente
CC := gcc
FLAGS := -Wall -Werror -pedantic -Wextra -fno-stack-protector -std=c11

all: $(PROGRAMS)

function: function.h function.c
	$(CC) $(FLAGS) -c function.c
servidor: servidor.c 
	$(CC) $(FLAGS) servidor.c -o servidorFTP
cliente: cliente.c 
	$(CC) $(FLAGS) cliente.c -o clienteFTP

clean: 
	rm -rf clienteFTP servidorFTP
