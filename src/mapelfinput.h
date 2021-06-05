#if !defined(LIBELF_H_)
#define LIBELF_H_

// size_t filesize;

typedef struct {
    Elf64_Ehdr    *ehdr;
    Elf64_Shdr    *shdr;
    Elf64_Phdr    *phdr;
    unsigned char **sdat;
    size_t        filesize;
} Elf64_File, * PELF;

PELF map_elf(const char *base);
void free_elf(PELF elf);

#endif // LIBELF_H_
