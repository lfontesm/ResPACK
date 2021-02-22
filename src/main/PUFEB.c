#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../mapfile.h"
#include "../ELF.h"
#include "../libelf.h"
#include "../estruturas.h"
#include "../util.h"
#include "../bitmap.h"
#include "../arvore.h"
#include "../buildelf.h"

int main(int argc, char **argv){
    if (argc < 2) {
        printf("Usage: %s <target file> \n", *argv);
        exit(EXIT_FAILURE);
    }
    const char *base;
    PELF elf;

    int inputfilefd = -1;
    base = map_to_mem(argv[1], &inputfilefd);
    elf = map_elf(base);

    int	outfilefd;

    if ((outfilefd = open("packed.lhel", O_CREAT | O_RDWR, 0744)) < 0) {
        perror("open");
        return 1;
    }
    write_ehdr(outfilefd);
    write_phdr(outfilefd);

    // write_encoded_tree(inputfilefd, outfilefd);

    close(inputfilefd);
    close(outfilefd);
    umap_file((char *)base, filesize);
    free_elf(elf);

    return 0;
}