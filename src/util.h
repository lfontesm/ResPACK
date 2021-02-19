#ifndef _H_UTIL
#define _H_UTIL

#include <stdio.h>
#include "estruturas.h"

typedef unsigned int uint;
typedef unsigned char uchar;


/**
 * Aloca na heap espaço para uma string de `LEN` caracteres
 * O espaço para o byte nulo é adicionado automaticamente
 */
#define ALOCA_STR(LEN) ((char*) malloc((LEN)+1))


// Debug

void printaCharInt(FILE* arq, uchar c);

void printaBits(FILE* saida, Bits bits);

void printaBitmap(FILE *saida, bitmap bits);

void printaCodigos(FILE *saida, Code *cods, uint n);

void printaFreqs(FILE *arq, int* freqVet);


#endif //_H_UTIL