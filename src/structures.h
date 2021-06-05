#ifndef _H_ESTRUTURAS
#define _H_ESTRUTURAS

#include <stdio.h>
#include "bitmap.h"

typedef unsigned int uint;
typedef unsigned char uchar;


// Structure to operato with bits in a byte
typedef struct bits {
    uint pos;   // bit counter
    uchar data; // the byte
} Bits;

// Operates over the byte

// Gets the value of a specific bit
#define GET_BIT(BITS, IDX) ( (BITS).data >> (7 - (IDX)) & 0x01 )

// Sets the value of a specific bit from the sequence to 1 if BIT == 1
#define SET_BIT(BITS, IDX, BIT)  ( (BITS).data = (BITS).data | ((BIT)&1) << (7 - (IDX)) )

// Adds a bit in sequence (sets the bit and increment the counter)
#define ADD_BIT(BITS, BIT) { \
        SET_BIT(BITS, (BITS).pos, BIT); \
        (BITS).pos++; \
    }

// Zeroes the bits and counter
#define ZERO_BITS(BITS) { (BITS).data = '\0'; (BITS).pos = 0; }


// Structure to store a code associated to a char
typedef struct code {
    uchar c;
    // int freq;
    bitmap bits;
} Code;


// Compares 2 codes by their frequency
int _compCodsFreq(const void *c1, const void *c2);

// Compares 2 codes by their chars
int _compCodsAlf(const void *c1, const void *c2);


// // Procuta um código pelo caractere em um vetor de códigos
// bitmap* pegaCodigo(Code* codes, uint ncods, uchar c);


#endif //_H_ESTRUTURAS