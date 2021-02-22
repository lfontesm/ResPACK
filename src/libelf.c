#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mapfile.h"
#include "ELF.h"
#include "libelf.h"

#define PRINT_TYPE(a, cpr) if ((a) == (cpr)) puts(#cpr)
#define MALLOC_ERR(a) {puts((a)); exit(EXIT_FAILURE);}

// initialized in mapfile.c

PELF map_elf(const char *base){
    PELF elf = malloc(sizeof(Elf64_File));
    if (!elf) 
        MALLOC_ERR("Failed to allocate space for elf structure");
    elf->filesize = filesize;

    /* Copy the Elf Header from file in memory to our user defined struct*/
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)base;

    elf->ehdr = malloc(sizeof(Elf64_Ehdr));
    if (!elf->ehdr)
        MALLOC_ERR("Failed to allocate space for Elf Header");

    Elf64_Half ehsize = ehdr->e_ehsize;
    memset(elf->ehdr, 0, ehsize);
    memcpy(elf->ehdr, base, ehsize);

    if (ehdr->e_shoff == 0){
        puts("File doesn't have a section header\n");
        goto mapphdr;
    }
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==


    /* Find the offset in the section header table of the entry associated with the section name string table.  */
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==
    Elf64_Half shentsize = ehdr->e_shentsize;
    Elf64_Half shstrndx  = ehdr->e_shstrndx;
    Elf64_Off  shoff     = ehdr->e_shoff;
    
    Elf64_Off shstrtabentOff = (shstrndx * shentsize) + shoff;
    printf("shstrtabent: 0x%08llx\n", shstrtabentOff);
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==

    
    /* Find the offset of the stringtable in file */
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==
    Elf64_Shdr *strtabent   = (Elf64_Shdr *)(base + shstrtabentOff);
    Elf64_Off   shstrtabOff = strtabent->sh_offset;
    printf("Firt byte of shstrtab starts @ 0x%08llx\n", shstrtabOff);
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==


    /* Allocate space for Section Headers and their data */
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==
    Elf64_Half shnum = ehdr->e_shnum;
    elf->shdr = malloc(sizeof(Elf64_Shdr) * shnum);
    if (!elf->shdr)
        MALLOC_ERR("Failed to allocate space for Section Headers");
    memset(elf->shdr, 0, sizeof(Elf64_Shdr) * shnum);
    elf->sdat = malloc(sizeof(u_char *) * shnum);
    if (!elf->sdat)
        MALLOC_ERR("Failed to allocate space for data of Section Headers");
    memset(elf->sdat, 0, sizeof(u_char *) * shnum);
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==
    
    
    /* Iterate section headers and copy them and their data into their respective struct fields in the PELF struct*/
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==
    int indx;
    for (indx = 0; indx < shnum; indx++){
        Elf64_Shdr *shdr = (Elf64_Shdr *)(base + ((indx * shentsize) + shoff));
        Elf64_Word nameIdx = shdr->sh_name;
        char *shname = (char *)(base + nameIdx + shstrtabOff);

        memcpy(&(elf->shdr[indx]), shdr, shentsize);

        Elf64_Xword ssize = shdr->sh_size;
        printf("Name of section: %s\n", shname);
        printf("\t[+] Section starts @ offset 0x%08llx in file and has size of %llx\n", shdr->sh_offset, ssize);        
        
        if (shdr->sh_type == SHT_NOBITS){
            elf->sdat[indx] = (u_char *)0;
        }
        else {
            elf->sdat[indx] = malloc(ssize);
            if (!elf->sdat[indx])
                MALLOC_ERR("Failed to allocate space for a specific section while copying them into sdat[indx]");

            memset(elf->sdat[indx], 0, ssize);
            // memcpy(elf->sdat[indx], (base + elf->shdr[indx].sh_offset), ssize);
        }
    }
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==

   
mapphdr: ;
    /* Iterate program headers and copy the segments tagged as CODE into the struct */
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==
    Elf64_Off  phoff     = ehdr->e_phoff;
    Elf64_Half phentsize = ehdr->e_phentsize;
    Elf64_Half phnum     = ehdr->e_phnum;

    if (!phoff){
        puts("File doesn't have a program header");
        return NULL;
    }

    elf->phdr = malloc(phentsize * phnum);
    if (!elf->phdr)
        MALLOC_ERR("Failed to allocate space for program headers");
    memset(elf->phdr, 0, phentsize * phnum);

    Elf64_Off  phvaddr = (Elf64_Off)(base + phoff);
    for (indx = 0; indx < phnum; indx++){
        Elf64_Phdr *phdr = (Elf64_Phdr *)(phvaddr + (phentsize * indx));

        printf("Type of segment @ 0x%llx: \n", phdr->p_offset);
        if (phdr->p_type == PT_NULL)
            printf("\t PT_NULL\n");
        if (phdr->p_type == PT_LOAD)
            printf("\t PT_LOAD\n");
        if (phdr->p_type == PT_DYNAMIC)
            printf("\t PT_DYNAMIC\n");
        if (phdr->p_type == PT_INTERP)
            printf("\t PT_INTERP\n");
        if (phdr->p_type == PT_NOTE)
            printf("\t PT_NOTE\n");
        if (phdr->p_type == PT_SHLIB)
            printf("\t PT_SHLIB\n");
        if (phdr->p_type == PT_PHDR)
            printf("\t PT_PHDR\n");
        if (phdr->p_type == PT_LOPROC)
            printf("\t PT_LOPROC\n");
        if (phdr->p_type == PT_HIPROC)
            printf("\t PT_HIPROC\n");
        if (phdr->p_type == PT_GNU_EH_FRAME)
            printf("\t PT_GNU_EH_FRAME\n");
        if (phdr->p_type == PT_GNU_PROPERTY)
            printf("\t PT_GNU_PROPERTY\n");
        if (phdr->p_type == PT_GNU_STACK)
            printf("\t PT_GNU_STACK\n");
    
        printf("\t[*]  W  |  R  |  X\n");
        printf("\t[*] [%d] | [%d] | [%d]\n", (phdr->p_flags >> 1) & 1, (phdr->p_flags >> 2) & 1, phdr->p_flags & 1);
        printf("\tSize of loaded file in memory: 0x%08llx\n", phdr->p_memsz);
        printf("\tSize of loaded file in disk: 0x%08llx\n", phdr->p_filesz);
        printf("\tVaddr: 0x%08llx || Paddr: 0x%08llx\n", phdr->p_vaddr, phdr->p_paddr);
        printf("\tAlignement: %llu\n", phdr->p_align);

        if (phdr->p_type == PT_LOAD){
            memcpy(&(elf->phdr[indx]), phdr, phentsize);
        }

    }
    // ==+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+====+_+==
    

    return elf;
}

void free_elf(PELF elf){
    if (!elf) return;

    if (elf->ehdr){
        if (elf->shdr){
            for (int i = 0; i < elf->ehdr->e_shnum; i++)
                if (elf->sdat[i]) free(elf->sdat[i]);

            free(elf->shdr);
        }
        free(elf->ehdr);
    }
    
    if (elf->phdr)
        free(elf->phdr);
    if (elf->sdat)
        free(elf->sdat);
    free(elf);
}