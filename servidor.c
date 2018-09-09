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

    // ./servidorFTP porto_servidor tam_buffer

int main(int argc, char **argv)
{
    //  Verifica quantidade de argumentos   //
     if(argc != 3)   {
         fprintf(stderr, "Utilização \t./servidorFTP porto_servidor tam_buffer\n");
         return EXIT_FAILURE;
     }

    //  Abre o endpoint de comunicação do servidor  //
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);                                   // Cria o descritor de arquivo do endpoint.
    if(sockfd == -1)    {                                                           // Verifica erro na criação do socket.
        perror("socket");                                                           // Imprime o erro.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }

    //  Cria a estrutura do endereçamento do servidor   //
    struct sockaddr_in server;                                                      // Estrutura que guarda as informações do servidor.
    server.sin_family = AF_INET;                                                    // A comunicação dar-se-a pelo protocolo IPv4.
    server.sin_addr.s_addr = INADDR_ANY;                                            // O servidor se conectará a todos os endereços da máquina.
    server.sin_port = htons(atoi(argv[1]));                                         // O servidor utilizará a porta passada como argumento.
    socklen_t slen = sizeof(server);                                                // Recebe o tamanho da estrutura do servidor.

    //  Realiza a conexão do endpoint à porta   //
    if(bind(sockfd, (struct sockaddr *) &server, (socklen_t) sizeof(server)) == -1) {//Verifica erro em bind.
        perror("bind");                                                             // Imprime o erro.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }

    //  Espera novas conexões   //
    if(listen(sockfd, 30) == -1)    {                                               // Verifica erro em listen.
        perror("listen");                                                           // Imprime o erro.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }

    //  Aceita nova conexão //
    int clifd = accept(sockfd, (struct sockaddr *) &server, (socklen_t *) &slen);
    if(clifd == -1)  {                                                              // Verifica erro em accept.
        perror("accept");                                                           // Imprime em caso de erro em accept.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }

    uint16_t bufSize = atoi(argv[2]);                                               // Recebe como argumento o tamanho do buffer.

    // Recebe o nome do arquivo de leitura  //
    char *fileName = (char *) malloc(bufSize * sizeof(char));                       // Aloca espaço para o nome do arquivo.
    ssize_t recved = recv(clifd, (char *) fileName, 20, 0);                         // Recebe o nome do arquivo.
    if(recved <= 0)  {                                                              // Verifica erro no recebimento.
        perror("recv-name");                                                        // Imprime o erro.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }
    fileName[recved] = 0;                                                           // Limita o nome do arquivo.
    
    //  Inicia contagem do tempo de execução    //
    struct timeval begin;                                                           // Estrutura que recebe o tempo inicial.
    gettimeofday(&begin, NULL);                                                     // Recebe o tempo inicial.

    //  Abre o arquivo de leitura   //
    FILE *file = fopen(fileName, "r");                              
    if(file == NULL)    {                                                           // Verifica erro na abertura do arquivo.
        perror("fopen");                                                            // Imprime o erro.
        close(sockfd);                                                              // Fecha a conexão.
        return EXIT_FAILURE;                                                        // Encerra o programa com sinal de erro.
    }

    //  Envia buffer por buffer o arquivo   //
    uint64_t sentBytes = 0;                                                         // Guardará o número de bytes enviados.
    while(!feof(file))  {                                                           // Itera o arquivo até o final.
        char *buffer = (char *) malloc(bufSize * sizeof(char));                     // Cria buffer de leitura.
        ssize_t read = fread(buffer, sizeof(char), bufSize, file);                  // Lê cada bloco de tamanho bufSize do arquivo.
       
        ssize_t sent = send(clifd, (char *) buffer, read, 0);                       // Envia o buffer.
        if(sent == -1)  {                                                           // Verifica erro no envio do buffer.
            perror("send");                                                         // Imprime o erro.
            return EXIT_FAILURE;                                                    // Encerra o programa com sinal de erro.
        }

        sentBytes += sent;                                                          // Incrementa o número de bytes enviados.
        
        free(buffer);                                                               // Libera o espaço alocado para o buffer.
    }

    //  Termina contagem de tempo de execução   //
    struct timeval end;                                                             // Estrutura que recebe o tempo final.
    gettimeofday(&end, NULL);                                                       // Recebe o tempo final.

    double finalTime = convertTimeDouble(end, begin);                               // Converte tempo para double.

    //  Imprime o nome do arquivo e bytes enviados  //
    fprintf(stdout, "Enviados %lu bytes do arquivo %s.\n", sentBytes, fileName);
    fprintf(stdout, "Tempo = %f s", finalTime);

    free(fileName);                                                                 // Remove espaço alocado para o nome do arquivo.
    close(sockfd);                                                                  // Fecha o endpoint do socket.
    fclose(file);                                                                   // Fecha o arquivo.

    return EXIT_SUCCESS;                                                            // Encerra o programa com sinal de sucesso.
}
