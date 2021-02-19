#include "estruturas.h"

#include <stdio.h>


int _compCodsFreq(const void *c1, const void *c2) {
    const Code *cod1 = c1;
    const Code *cod2 = c2;
    return bitmapGetLength(cod1->bits) - bitmapGetLength(cod2->bits);
}

int _compCodsAlf(const void *c1, const void *c2) {
    const Code *cod1 = c1;
    const Code *cod2 = c2;
    return (int) (cod1->c - cod2->c);
}

// bitmap* pegaCodigo(Code* codigos, uint ncods, uchar c){
//     for (int i = 0; i < ncods; i++){
//         if (codigos[i].c == c)
//             return &codigos[i].bits;
//     }
//     return NULL;
// }
