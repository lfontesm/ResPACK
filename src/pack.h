#if !defined(COMPACTA_H_)
#define COMPACTA_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "structures.h"
#include "util.h"
#include "bitmap.h"
#include "tree.h"
#include "ELF.h"

void count_char(int* freqVet, int inputfilefd);

void organize_code (Code* codes, uint cods, bitmap* codesVec[256]);

uint encode_file(int inputfilefd, int outfilefd, bitmap* codesVec[256], uint cods);


#endif // COMPACTA_H_
