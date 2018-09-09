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
