#ifndef _H_UTIL
#define _H_UTIL

#include <stdio.h>
#include "structures.h"

typedef unsigned int uint;
typedef unsigned char uchar;


/**
 * Aloca na heap espaço para uma string de `LEN` caracteres
 * O espaço para o byte nulo é adicionado automaticamente
 * Allocate space in the heap to a string with `LEN` chars
 * the space to null byte is added automatically
 */
#define ALLOC_STR(LEN) ((char*) malloc((LEN)+1))


// Debug

void print_int_char(FILE* arq, uchar c);

void print_bits(FILE* outFile, Bits bits);

void print_bitmap(FILE *outFile, bitmap bits);

void print_codes(FILE *outFile, Code *cods, uint n);

void print_freqs(FILE *arq, int* freqVet);


#endif //_H_UTIL