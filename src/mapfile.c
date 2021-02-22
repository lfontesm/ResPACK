#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

// initialized in mapfile.c
size_t filesize;

char *map_to_mem(char *filepath, int *fd){
    *fd = open(filepath, O_RDONLY | O_SYNC);
    if (*fd == -1){
        perror("Failed to open target file");
        puts(strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct stat buf;
    int err = fstat(*fd, &buf);
    if (err == -1){
        perror("Failed to retrieve information about input file");
        exit(EXIT_FAILURE);
    }

    filesize = buf.st_size;
    char *base=(char *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, *fd, 0);
    if (base == MAP_FAILED) {
        perror("Failed to map file to mem");
        exit(EXIT_FAILURE);
    }

    printf("File addr: %p\n", base);
    // close(*fd);

    return base;
}

void umap_file(char *base, size_t filesize){
    int umap = munmap(base, filesize);
    if (umap == -1){
        perror("Failed to deallocate memory from opened file");
        exit(EXIT_FAILURE);
    }
}