#include <stdio.h>

#include "tree.h"


/* 
    TODO: Implement this whole file in assembly level and copy it onto the packed binary
    this file has the extension `.cpp` so the Makefile won't compile it and the syntax highlight will still show on VSCode
    in order to generate the .asm file, change the extension back to `.c` so that the simble names won't be mangled
    and run the command: `gcc unpack.c -o unpack.asm -S -masm=intel`
 */

// Fetches netx bit in buffer, loads next byte when needed, and
// indicates the end of the file and code through `eof` and `reading` flags
uchar getBit(Bits* buf, uchar* nextinbuf, FILE* arq, int* eof, int* reading, uint remainder){

    // Fetches next bit and increment bit counter
    uchar ret = GET_BIT(*buf, buf->pos++);

    // In case it's the last byte
    if (*eof) {
        // In case the last bits have been read
        if (buf->pos == remainder)
            *reading = 0;
    }

    // In case it's not the last byte, and it's already been read 8 bits
    // loads new byte from file
    else if (buf->pos == 8) {
        buf->data = *nextinbuf;
        buf->pos = 0;

        *nextinbuf = fgetc(arq);
        // In case it's EOF, indicates to stop reading loop
        *eof = feof(arq);
    }

    return ret;
}

void deserialize(Tree* tree, FILE* inputfiledf, FILE* outfilefd, uint remainder){
    Bits buffer = (Bits){0, '\0'};
    uchar nextinbuf = '\0';

    int eof = 0;
    int reading = 1;

    // Reads first byte and stop in case of EOF
    buffer.data = fgetc(inputfiledf);
    if (feof(inputfiledf)) return;

    nextinbuf = fgetc(inputfiledf);
    eof = feof(inputfiledf);

    Tree* no;
    int isBranch;

    // In case of a single leaf (1 character in the whole file)
    if (tree_is_leaf(tree)) {
        const uchar chr = tree_char(tree);
        while(reading){
            getBit(&buffer, &nextinbuf, inputfiledf, &eof, &reading, remainder);
            fputc(chr, outfilefd);
        }
        return;
    }

    // In case it's an actual tree
    while(reading) {
        isBranch = 1;
        no = tree; // TODO check existence
        // Reads until finding a leaf or end of sequence
        while (isBranch && reading) {
            uchar bit = getBit(&buffer, &nextinbuf, inputfiledf, &eof, &reading, remainder);
            no = walk(no, bit, &isBranch);
        }
        // In case it stoped reading in the middle of the tree, emit error
        if (isBranch) {
            fprintf(stderr, "Error while deserializing binary sequency\n");
            fclose(outfilefd);
            exit(1);
        }
        fputc(tree_char(no), outfilefd);
    }
}


int main(int argc, char **argv) {
    // Guarantee 2 params
    // if (argc <= 2) {
    //     fprintf(stderr, "Uso: %s <entrada> <outFile>\n", argv[0]);
    //     return 0;
    // }

    char* ARQ_ENTRADA = argv[1];
    char* ARQ_SAIDA = argv[2];

    FILE* inputfiledf = fopen(ARQ_ENTRADA, "r");
    // if (!inputfiledf) {
    //     fprintf(stderr, "Falha ao abrir '%s' para leitura\n", ARQ_ENTRADA);
    //     return 1;
    // }
    FILE* outfilefd = fopen(ARQ_SAIDA, "w");
    // if (!outfilefd) {
    //     fprintf(stderr, "Falha ao abrir '%s' para escrita\n", ARQ_SAIDA);
    //     return 1;
    // }

    // fprintf(stderr, "Descompactando\n");

    // Reads the byte that indicates the amount of bits left in the last byte
    uchar remainder = fgetc(inputfiledf);
    if (feof(inputfiledf))
        return 1;

    // fprintf(stderr, "remainder: %u\n", remainder); /// DEBUG

    Tree* huff = tree_deserialize(inputfiledf);

    // print_tree(stderr, huff); fprintf(stderr, "\n"); /// DEBUG

    deserialize(huff, inputfiledf, outfilefd, (uint)remainder);

    fclose(inputfiledf);
    fclose(outfilefd);
    free_tree(huff);

    return 0;
}
