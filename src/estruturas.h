#ifndef _H_ESTRUTURAS
#define _H_ESTRUTURAS

#include <stdio.h>
#include "bitmap.h"

typedef unsigned int uint;
typedef unsigned char uchar;


// Estrutura para operar com bits em um byte
typedef struct bits {
    uint pos;   // contador de bits
    uchar data; // o byte
} Bits;

// Operações sobre o byte

// Pega um valor de um bit específico
#define PEGA_BIT(BITS, IDX) ( (BITS).data >> (7 - (IDX)) & 0x01 )

// Seta o valor de um bit específico da sequência para 1 se BIT == 1
#define SETA_BIT(BITS, IDX, BIT)  ( (BITS).data = (BITS).data | ((BIT)&1) << (7 - (IDX)) )

// Adiciona um bit na sequência (seta o bit e incrementa o contador)
#define ADD_BIT(BITS, BIT) { \
        SETA_BIT(BITS, (BITS).pos, BIT); \
        (BITS).pos++; \
    }

// Zera os bits e o contador
#define ZERA_BITS(BITS) { (BITS).data = '\0'; (BITS).pos = 0; }


// Estrutura para armazenar um código associado a um caractere
typedef struct codigo {
    uchar c;
    // int freq;
    bitmap bits;
} Code;


// Compara dois códigos pela frequência
int _compCodsFreq(const void *c1, const void *c2);

// Compara dois códigos pelo caractere
int _compCodsAlf(const void *c1, const void *c2);


// // Procuta um código pelo caractere em um vetor de códigos
// bitmap* pegaCodigo(Code* codigos, uint ncods, uchar c);


#endif //_H_ESTRUTURAS