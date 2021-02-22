#include "buildelf.h"

// extern u_int64_t		loader_size;
// extern u_int64_t		infos_size;
// extern void		entry_loader(void);

/* Defining the structs we will use in this file to build the new elf file */
#define MAX_PHDR_NUM 3

typedef struct {
    Elf64_Ehdr ehdr;
    Elf64_Off  offsetinfile;
} NEW_EHDR;

typedef struct {
    Elf64_Phdr phdrlist[MAX_PHDR_NUM];
    Elf64_Off  offsetinfile[MAX_PHDR_NUM];
} NEW_PHDR;

int offset;

/* Initialize the structs */
NEW_EHDR newehdr = {
    .ehdr = {
        .e_ident     = {'\x7f','\x45','\x4c','\x46','\x02','\x01','\x01','\x03','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00'},	/* ELF "magic number" */
        .e_type      = ET_EXEC,
        .e_machine   = 0x3E,
        .e_version   = 0x01,
        .e_entry     = 0,	  /* Entry point virtual address */
        .e_phoff     = 64,	  /* Program header table file offset */
        .e_shoff	 = 0,     /* Section header table file offset */
        .e_flags     = 0,
        .e_ehsize    = 64,
        .e_phentsize = 56,
        .e_phnum     = 3,
        .e_shentsize = 64,
        .e_shnum     = 0,
        .e_shstrndx  = 0
    },
    .offsetinfile = 0
};

NEW_PHDR newphdr = {
    .phdrlist = {[0 ... 2] = {
        .p_type  = PT_LOAD,
        .p_flags = 0,
        .p_offset = 0,
        .p_vaddr = 0,
        .p_paddr = 0,
        .p_filesz = 0,
        .p_memsz = 0,
        .p_align = 0
    }},
    .offsetinfile = {[0 ... 2] = 0}
};

void write_ehdr(int outfilefd){
    offset = write(outfilefd, &(newehdr.ehdr), sizeof(Elf64_Ehdr));
    if (offset == -1) {
        perror("Operation write failed: ");
        exit(EXIT_FAILURE);
    }
    if (offset != sizeof(newehdr.ehdr)) 
        puts("Writing of EHDR was probably not succesful");
        
}

Elf64_Phdr init_a_phdr(int indx, Elf64_Word p_type, Elf64_Word p_flags,
                        Elf64_Off p_offset, Elf64_Addr p_addr, Elf64_Xword p_size, 
                        Elf64_Xword p_align, int isheap){
    Elf64_Phdr phdr = newphdr.phdrlist[indx] = (Elf64_Phdr) {
        .p_type = p_type,
        .p_flags = p_flags,
        .p_offset = p_offset,
        .p_vaddr = p_addr,
        .p_paddr = p_addr,
        .p_memsz = p_size,
        .p_align = p_align
    };

    if (!isheap)
        phdr.p_filesz = newphdr.phdrlist[indx].p_filesz = p_size;

    return phdr;
}

void write_phdr(int outfilefd){
    int nwrite;
    Elf64_Phdr phdr_exec = init_a_phdr(0, PT_LOAD, PF_R|PF_X|PF_W, 0x1234, 0x400000, 0xff, 0x1000, 0);
    newphdr.offsetinfile[0] = 64;
    Elf64_Phdr phdr_heap = init_a_phdr(1, PT_LOAD, PF_W|PF_R, 0x4321, 0x460000, 0xff, 0x1000, 1);
    newphdr.offsetinfile[1] = 64+56;
    Elf64_Phdr phdr_stack = init_a_phdr(2, PT_GNU_STACK, PF_W|PF_R, 0, 0, 0x0, 0x10, 0);
    newphdr.offsetinfile[2] = 64+56+56;    


    nwrite = write(outfilefd, &phdr_exec, sizeof(phdr_exec));
    if (nwrite == -1){
        perror("Operation write failed: ");
        exit(EXIT_FAILURE);
    }
    if (nwrite != sizeof(phdr_exec)) 
        puts("Writing of phdr was probably not succesful");

    offset += nwrite;

    nwrite = write(outfilefd, &phdr_heap, sizeof(phdr_heap));
    if (nwrite == -1){
        perror("Operation write failed: ");
        exit(EXIT_FAILURE);
    }
    if (nwrite != sizeof(phdr_heap)) 
        puts("Writing of phdr heap was probably not succesful");

    offset += nwrite;

    nwrite = write(outfilefd, &phdr_stack, sizeof(phdr_stack));
    if (nwrite == -1){
        perror("Operation write failed: ");
        exit(EXIT_FAILURE);
    }
    if (nwrite != sizeof(phdr_stack)) 
        puts("Writing of phdr_stack was probably not succesful");

    offset += nwrite;
}