
#include "util.h"

#include <stdio.h>
#include <ctype.h>
#include "estruturas.h"


// Debug

/** 
 * Imprime o caractere se for possível e seu inteiro correspondente
 */
void printaCharInt(FILE* arq, uchar c){
    if (isprint(c)) {
        fprintf(arq, "[%c]", c);
    }
    fprintf(arq, "(%d)", (int)c);
}


void printaBits(FILE* saida, Bits bits) {
    for (int i = 0; i < bits.pos; i++) {
        fprintf(saida, "%d",  PEGA_BIT(bits, i));
    }
}

void printaBitmap(FILE *saida, bitmap bits){
    uint tam = bitmapGetLength(bits);
    for (int i = 0; i < tam; i++){
        fprintf(saida, "%d", bitmapGetBit(bits, i));
    }
}

void printaCodigos(FILE *saida, Code *cods, uint n){
    for (uint i = 0; i < n; i++){
        uchar chr = cods[i].c;
        fprintf(saida, "[%4u] ", (int) chr);
        if (isprint(chr))
            fprintf(saida, "(%4c): ", chr);
        else
            fprintf(saida, "(    ): ");
        printaBitmap(saida, cods[i].bits);
        fprintf(saida, "\n");
    }
}


void printaFreqs(FILE *arq, int* freqVet){
    for (int i = 0; i < 64; i++) {
        int n = i*4;
        fprintf(arq, "| ");
        for (int j = 0; j < 4; j++) {
            fprintf(arq, "| %8d | %3d |", (int)n+j, freqVet[n+j]);
        }
        fprintf(arq, "\n");
    }
}
