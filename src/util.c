
#include "util.h"

#include <stdio.h>
#include <ctype.h>
#include "structures.h"


// Debug

/** 
 * Imprime o caractere se for possível e seu inteiro correspondente
 */
void print_int_char(FILE* arq, uchar c){
    if (isprint(c)) {
        fprintf(arq, "[%c]", c);
    }
    fprintf(arq, "(%d)", (int)c);
}


void print_bits(FILE* outFile, Bits bits) {
    for (int i = 0; i < bits.pos; i++) {
        fprintf(outFile, "%d",  GET_BIT(bits, i));
    }
}

void print_bitmap(FILE *outFile, bitmap bits){
    uint tam = bitmapGetLength(bits);
    for (int i = 0; i < tam; i++){
        fprintf(outFile, "%d", bitmapGetBit(bits, i));
    }
}

void print_codes(FILE *outFile, Code *cods, uint n){
    for (uint i = 0; i < n; i++){
        uchar chr = cods[i].c;
        fprintf(outFile, "[%4u] ", (int) chr);
        if (isprint(chr))
            fprintf(outFile, "(%4c): ", chr);
        else
            fprintf(outFile, "(    ): ");
        print_bitmap(outFile, cods[i].bits);
        fprintf(outFile, "\n");
    }
}


void print_freqs(FILE *arq, int* freqVet){
    for (int i = 0; i < 64; i++) {
        int n = i*4;
        fprintf(arq, "| ");
        for (int j = 0; j < 4; j++) {
            fprintf(arq, "| %8d | %3d |", (int)n+j, freqVet[n+j]);
        }
        fprintf(arq, "\n");
    }
}
