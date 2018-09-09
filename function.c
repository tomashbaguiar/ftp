#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include "function.h"

struct timeval
convertTime(struct timeval end, struct timeval begin)
{
    //  Calcula o tempo total   //
    struct timeval finalTime;
    long int seconds = (end.tv_sec - begin.tv_sec);
    long int microseconds = (end.tv_usec - begin.tv_usec);
    long int timeConv = (seconds * 1000000) + microseconds;
    finalTime.tv_sec = timeConv / 1000000;
    finalTime.tv_usec = timeConv - (finalTime.tv_sec * 1000000);

    return finalTime;                                                                   // Retorna o tempo real total.
}

double
convertTimeDouble(struct timeval end, struct timeval begin)
{
    //  Calcula o tempo total   //
    uint64_t init = begin.tv_sec * 1000000 + begin.tv_usec;                             // Recebe o valor absoluto do tempo inicial.
    uint64_t final = end.tv_sec * 1000000 + end.tv_usec;                                // Recebe o valor absoluto do tempo final.
    uint64_t time = final - init;                                                       // Calcula o valor absoluto do tempo total.

    //  Converte para double a estrutura timeval    //
    double finalTime = (double) time;                                                   // Recebe o tempo total absoluto.
    finalTime /= 1000000.0;                                                             // Converte para tempo real.

    return finalTime;                                                                   // Retorna o tempo real total.
}
