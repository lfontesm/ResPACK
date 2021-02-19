#include "buildelf.h"

// extern u_int64_t		loader_size;
// extern u_int64_t		infos_size;
// extern void		entry_loader(void);

extern Elf64_Ehdr newelfEhdr;

void dump_ehdr(int outfilefd, Elf64_Ehdr newehdr){
    int nwrite;
    nwrite = write(outfilefd, &newehdr, sizeof(newehdr));
    if (nwrite == -1) {
        perror("Operation write failed: ");
        exit(EXIT_FAILURE);
    }
    if (nwrite != sizeof(newehdr)) 
        puts("Writing was probably not succesful");
        
}