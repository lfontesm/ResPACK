#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../basemem.h"
#include "../ELF.h"
#include "../mapelfinput.h"
#include "../structures.h"
#include "../util.h"
#include "../bitmap.h"
#include "../tree.h"
#include "../buildelfoutput.h"

int main(int argc, char **argv){
    if (argc < 2) {
        printf("Usage: %s <target file> \n", *argv);
        exit(EXIT_FAILURE);
    }
    const char *base;

    int inputfilefd = -1;
    // Gets the base VA for the input file (the one to be packed)
    base = allocate_base(argv[1], &inputfilefd);

    int	outfilefd;
    // Opens output file for writing (the soon to be packed binary)
    if ((outfilefd = open("packed.lhel", O_CREAT | O_RDWR, 0744)) < 0) {
        perror("open");
        return 1;
    }
    // Starts by writing both the Elf_Header and the Program_Section header
    // They are lacking completeness. We will be updating them as we go
    write_ehdr(outfilefd);
    write_phdr(outfilefd);

    // Write the encoded huffman tree on the output file along with the structures needed
    // to unpack the binary
    write_encoded_tree(inputfilefd, outfilefd);

    // TODO: write unpacking routine in ASM and inprint it on the packed binary

    close(inputfilefd);
    close(outfilefd);
    umap_file((char *)base, filesize);

    return 0;
}