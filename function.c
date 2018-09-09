#include <sys/time.h>
#include <stdint.h>
#include <stdlib.h>
#include "function.h"

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
