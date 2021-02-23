#if !defined(COMPACTA_H_)
#define COMPACTA_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "estruturas.h"
#include "util.h"
#include "bitmap.h"
#include "arvore.h"
#include "ELF.h"

void count_char(int* freqVet, int inputfilefd);

void organizaCodigos (Code* codigos, uint cods, bitmap* codigosVet[256]);

uint codificaArquivo(int inputfilefd, int outfilefd, bitmap* codigosVet[256], uint cods);


#endif // COMPACTA_H_
