#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#include "function.h"

    // ./clienteFTP host_do_servidor porto_servidor nome_arquivo tam_buffer

int main(int argc, char **argv)
{
    //  Verifica quantidade de argumentos   //
    if(argc != 5)   {
        fprintf(stderr, "Utilização \t./clienteFTP host_do_servidor porto_servidor nome_arquivo tam_buffer\n");
        return EXIT_FAILURE;
    }

    //  Abre o endpoint de comunicação do cliente  //
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);                                   // Cria o descritor de arquivo do endpoint.
    if(sockfd == -1)    {                                                           // Verifica erro na criação do socket.
        perror("socket");                                                           // Imprime o erro.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }

    //  Cria a estrutura do endereçamento do servidor   //
    struct sockaddr_in server;                                                      // Estrutura que guarda as informações do servidor.
    server.sin_family = AF_INET;                                                    // A comunicação dar-se-a pelo protocolo IPv4.
    server.sin_port = htons(atoi(argv[2]));                                         // O servidor utilizará a porta passada como argumento.
    struct hostent *hp;	                                                            // Ponteiro para receber o ip/dominio.
	char *hostname = (char *) malloc(strlen(argv[1])*sizeof(char));
    strcpy(hostname, argv[1]);	                                                    // Recebe por parametro o ip/dominio
	if(!(hp = gethostbyname(hostname)))	                                            // Recebe o endereço de argv[1].
	{
		printf("%s was not resolved. gethostbyname:\n", hostname);
		exit(EXIT_FAILURE);
	}
    server.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr_list[0]))->s_addr;	    //Atualiza endereço do servidor (ip).

    //  Conecta ao servidor e verifica erro de conexão  //
    if(connect(sockfd, (struct sockaddr *) &server, (socklen_t) sizeof(server)) == -1)  {
        perror("connect");                                                          // Imprime o erro.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }
    
    //  Inicia contagem do tempo de execução    //
    struct timeval begin;                                                           // Estrutura que recebe o tempo inicial.
    gettimeofday(&begin, NULL);                                                     // Recebe o tempo inicial.

    //  Envia ao servidor o nome do arquivo //
    uint16_t bufSize = atoi(argv[4]);                                               // Recebe o tamanho do buffer.
    char *fileName = argv[3];                                                       // Recebe o nome do arquivo.
    if(send(sockfd, (char *) fileName, strlen(fileName), 0) == -1)  {               // Verifica e envia o buffer com o nome do arquivo.
        perror("send");                                                             // Imprime erro de envio.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }

    //  Abre o arquivo de escrita para receber do servidor  //
    FILE *file = fopen("output.txt", "w+");                                             
    if(file == NULL)    {                                                           // Verifica erro na abertura do arquivo.
        perror("fopen");                                                            // Imprime o erro.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }

    //  Recebe buffer a buffer o arquivo do servidor    //
    uint64_t bytesRecv = 0;                                                         // Guarda o número de bytes recebidos.
    ssize_t recved = 0;                                                             // Receberá o retorno de recv.
    char *buffer = (char *) malloc(bufSize * sizeof(char));                         // Aloca espaço para o buffer.
    while((recved = recv(sockfd, (char *) buffer, bufSize, 0)) > 0) {
        buffer[recved] = 0;                                                         // Limita o buffer.

        fwrite(buffer, sizeof(char), recved, file);                                 // Escreve no arquivo.

        bytesRecv += recved;                                                        // Atualiza o contador de bytes recebidos.
    }

    free(buffer);                                                                   // Libera o espaço alocado para o buffer.
    buffer = NULL;

    if(recved == -1)    {                                                           // Verifica erro no recebimento.
        perror("recv");                                                             // Imprime o erro.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }

    //  Termina contagem de tempo de execução   //
    struct timeval end;                                                             // Estrutura que recebe o tempo final.
    gettimeofday(&end, NULL);                                                       // Recebe o tempo final.
    struct timeval finalTime = convertTime(end, begin);                             // Converte o tempo para double.
    float fFinalTime = convertTimeDouble(end, begin);
    fprintf(stdout, "Buffer = %5u byte(s), %10.2f kbps (%lu bytes em %3u.%06u s).\n", bufSize, (float) (bytesRecv / fFinalTime), bytesRecv, (uint32_t) finalTime.tv_sec, (uint32_t) finalTime.tv_usec);

    close(sockfd);                                                                  // Fecha o endpoint do cliente.
    
    fclose(file);                                                                   // Fecha o arquivo de saída.
    file = NULL;

    return EXIT_SUCCESS;
}

