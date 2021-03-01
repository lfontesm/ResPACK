#include "buildelf.h"
#include "compacta.h"

extern u_int64_t        loader_size;
extern u_int64_t        infos_size;
extern void             entry_loader(void);

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
        .e_entry     = 0x445560,  /* Entry point virtual address */
        .e_phoff     = 64,	      /* Program header table file offset */
        .e_shoff	 = 0,         /* Section header table file offset */
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

NEW_PHDR newphdrlist = {
    .phdrlist = {[0 ... 2] = {
        .p_type   = PT_LOAD,
        .p_flags  = 0,
        .p_offset = 0,
        .p_vaddr  = 0,
        .p_paddr  = 0,
        .p_filesz = 0,
        .p_memsz  = 0,
        .p_align  = 0
    }},
    .offsetinfile = {[0 ... 2] = 0}
};

void set_fileds_offset(int fd, off_t offset, int whence){
    int seekoffset;
    if ((seekoffset = lseek(fd, offset, whence)) == -1){
        perror("Lseek failed");
        exit(EXIT_FAILURE);
    }
}

/* 
    Write ELF header
 */
void write_ehdr(int outfilefd){
    offset = write(outfilefd, &(newehdr.ehdr), sizeof(Elf64_Ehdr));
    if (offset == -1) {
        perror("Operation write failed: ");
        exit(EXIT_FAILURE);
    }
    if (offset != sizeof(newehdr.ehdr)) 
        puts("Writing of EHDR was probably not succesful");
        
}

/* 
    Initialize a program header
 */
Elf64_Phdr init_a_phdr(int indx, Elf64_Word p_type, Elf64_Word p_flags,
                        Elf64_Off p_offset, Elf64_Addr p_addr, Elf64_Xword p_size, 
                        Elf64_Xword p_align, int isheap){
    Elf64_Phdr phdr = newphdrlist.phdrlist[indx] = (Elf64_Phdr) {
        .p_type = p_type,
        .p_flags = p_flags,
        .p_offset = p_offset,
        .p_vaddr = p_addr,
        .p_paddr = p_addr,
        .p_memsz = p_size,
        .p_align = p_align
    };

    if (!isheap)
        phdr.p_filesz = newphdrlist.phdrlist[indx].p_filesz = p_size;

    return phdr;
}

void _write_phdr(int outfilefd, Elf64_Phdr phdr){
    int nwrite;
    nwrite = write(outfilefd, &phdr, sizeof(phdr));
    if (nwrite == -1){
        perror("Operation write failed: ");
        exit(EXIT_FAILURE);
    }
    if (nwrite != sizeof(phdr)) 
        puts("Writing of phdr was probably not succesful");

    offset += nwrite;

    // nwrite = write(outfilefd, &phdr_heap, sizeof(phdr_heap));
    // if (nwrite == -1){
    //     perror("Operation write failed: ");
    //     exit(EXIT_FAILURE);
    // }
    // if (nwrite != sizeof(phdr_heap)) 
    //     puts("Writing of phdr heap was probably not succesful");

    // offset += nwrite;

    // nwrite = write(outfilefd, &phdr_stack, sizeof(phdr_stack));
    // if (nwrite == -1){
    //     perror("Operation write failed: ");
    //     exit(EXIT_FAILURE);
    // }
    // if (nwrite != sizeof(phdr_stack)) 
    //     puts("Writing of phdr_stack was probably not succesful");

    // offset += nwrite;
}

/* 
    Write program headers
 */
Elf64_Phdr phdr_exec;
Elf64_Phdr phdr_heap;
Elf64_Phdr phdr_stack;
void write_phdr(int outfilefd){
    int nwrite;
    phdr_exec = init_a_phdr(0, PT_LOAD, PF_R|PF_X, 0x0, 0x400000, 0xff, 0x1000, 0);
    newphdrlist.offsetinfile[0] = 64;
    phdr_heap = init_a_phdr(1, PT_LOAD, PF_W|PF_R, 0x0, 0x460000, 0xff, 0x1000, 1);
    newphdrlist.offsetinfile[1] = 64+56;
    phdr_stack = init_a_phdr(2, PT_GNU_STACK, PF_W|PF_R, 0, 0, 0x0, 0x10, 0);
    newphdrlist.offsetinfile[2] = 64+56+56;    

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

/* 
    A copy of kernel's strstr but using fixed size buffers to avoid
    null byte shenanigans
 */
char *_strstr(char *haystack, char *needle, off_t max_size, int len){
    size_t l1, l2;

	l2 = 0;
	l1 = max_size;
	while (l1 >= l2) {
		l1--;
		if (!memcmp(haystack, needle, len))
			return (char *)haystack;
		haystack++;
	}
	return NULL;
}

/* 
    After we get the value of `remainder` we need to rewrite the stub with its value
 */
void search_and_write_in_stub(int outfilefd, char *stub, uchar *remainder){
    // Return to the beginning of the file
    int seekoffset;
    if ((seekoffset = lseek(outfilefd, 0, SEEK_SET)) == -1){
        perror("Lseek failed");
        exit(EXIT_FAILURE);
    }

    struct stat statbuf;
    int err = fstat(outfilefd, &statbuf);
    if (err == -1){
        perror("Failed to retrieve information about output file");
        exit(EXIT_FAILURE);
    }

    off_t filesize = statbuf.st_size;
    char *buf = malloc(sizeof(char) * filesize);

    // Read the whole file into a buffer
    ssize_t nread = read(outfilefd, buf, filesize);
    if (nread == -1){
        perror("Failed while looking for stub");
        exit(EXIT_FAILURE);
    }

    // Search for the stub in aforementioned buffer
    char *ptr;
    if((ptr = _strstr(buf, stub, filesize, strlen(stub))) != NULL)
        ptr[strlen(stub) - 1] = *remainder;

    if ((seekoffset = lseek(outfilefd, 0, SEEK_SET)) == -1){
        perror("Lseek failed");
        exit(EXIT_FAILURE);
    }

    ssize_t nwrite = write(outfilefd, buf, filesize);
    if (nwrite == -1){
        perror("Failed to rewrite the stub");
        exit(EXIT_FAILURE);
    }

    free(buf);
}

/* 
    Update the size from the program header table of index `indx`. If `updateboth` is set,
    then both the memsz and filesz attributes will be updated, otherwise only memsz
 */

void ajust_phdr_size(int outfilefd, int indx, Elf64_Xword size, int updateboth){
    Elf64_Phdr oldphdr = newphdrlist.phdrlist[indx];
    Elf64_Phdr newphdr = init_a_phdr(indx, oldphdr.p_type, oldphdr.p_flags, oldphdr.p_offset, oldphdr.p_vaddr, size, oldphdr.p_align, updateboth);
    
    set_fileds_offset(outfilefd, 0, SEEK_SET);
    write_ehdr(outfilefd);
    
    for (size_t i = 0; i < MAX_PHDR_NUM; i++)
    {
        if (i == indx)
            _write_phdr(outfilefd, newphdr);
        else
            _write_phdr(outfilefd, newphdrlist.phdrlist[i]);

    }
}

extern Elf64_Off segmentsize;

off_t pad_zero(int outfilefd){
    ssize_t nwrite;
    uchar c = 0;

    struct stat statbuf;
    int err = fstat(outfilefd, &statbuf);
    if (err == -1){
        perror("Failed to retrieve information about output file");
        exit(EXIT_FAILURE);
    }

    ssize_t newsize = statbuf.st_size;
    printf("Size in pad func:%ld\n", statbuf.st_size);
    while ((newsize % 4) != 0){
        newsize += nwrite = write(outfilefd, &c, sizeof(uchar));
        if (nwrite == -1){
            perror("Failed to pad the file");
            exit(EXIT_FAILURE);
        }
    }
    return newsize;
}

void write_encoded_tree(int inputfilefd, int outfilefd) {
    // int nwrite;

    // Return to the beginning of the file
    int seekoffset;
    if ((seekoffset = lseek(inputfilefd, 0, SEEK_SET)) == -1){
        perror("Lseek failed");
        exit(EXIT_FAILURE);
    }

    // Init frequency vector
    int freq[256] = {0};
    count_char(freq, inputfilefd);
    if ((seekoffset = lseek(inputfilefd, 0, SEEK_SET)) == -1){
        perror("Lseek failed");
        exit(EXIT_FAILURE);
    }

    // printaFreqs(stderr, freq);    /// DEBUG

    // Cria a lista de folhas a partir da tabela de frequências
    ListaArv* lista = Lista_cria();
    for (int i = 0; i < 256; i++) {
        int fq = freq[i];
        if (fq){
            lista = Lista_insOrd(lista, criaFolha((uchar)i, fq));
        }
    }

    // Lista_imprime(stderr, lista); fprintf(stderr, "\n");  /// DEBUG

    // Obtém a árvore ótima e libera a lista (agora vazia)
    Arv* huff = constroiArvoreOtima(lista);
    Lista_libera(lista);


    // Arv_imprime(stderr, huff); fprintf(stderr, "\n\n");   /// DEBUG

    // exit(1);

    // Varre a árvore para criar a tabela de códigos
    Code codigos[256]; uint cods = 0;
    Arv_criaCodigos(huff, codigos, &cods);

    // // Imprime ćodigos em ordem alfabética
    // qsort(codigos, cods, sizeof(Code), _compCodsAlf);
    // printaCodigos(stderr, codigos, cods); fprintf(stderr, "\n");    /// DEBUG

    // // Coloca em ordem de menor quantidade de bits (maior frequência)
    // qsort(codigos, cods, sizeof(Code), _compCodsFreq);
    // printaCodigos(stderr, codigos, cods); fprintf(stderr, "\n");    /// DEBUG

    // Coloca os bitmaps dos códigos em um vetor associando pelas posições
    bitmap* codigosVet[256] = {NULL};
    organizaCodigos(codigos, cods, codigosVet);

    // Stub of output file
    ssize_t stubn = write(outfilefd, "LHEL!0", strlen("LHEL!0"));
    if (stubn != strlen("LHEL!0")) {
        puts("Something went wrong when writing the LHEL! stub");
        exit(EXIT_FAILURE);
    }

    offset += stubn;

    // Codifica uma árvore para um bitmap
    bitmap arvSer = Arv_serializa(huff);
    // printaBitmap(stderr, arvSer); fprintf(stderr, "\n\n");    /// DEBUG

    // Escreve a árvore codificada no arquivo
    {
        uchar* arvData = bitmapGetContents(arvSer);
        int k = (bitmapGetLength(arvSer)+7)/8;
        for (int i = 0; i < k; i++)
            write(outfilefd, &arvData[i], sizeof(arvData[i]));
        // fputc(arvData[i], arqSaida);
    }

    Arv_libera(huff);
    bitmapFree(arvSer);

    // Codifica o arquivo de entrada, escrevendo
    uchar remainder = codificaArquivo(inputfilefd, outfilefd, codigosVet, cods);
    // printf("%u || %u\n", remainder, (uchar)remainder);

    // fprintf(stderr, "resto: %u\n", resto);  /// DEBUG

    // Escreve a quantidade de bits do último byte no primeiro byte do arquivo
    // lseek(outfilefd, offset -1, SEEK_SET);
    // write(outfilefd, &resto, sizeof(uchar));
    // fputc((uchar)resto, arqSaida);
    // rewind(arqSaida);
    // int a = search_and_write_in_stub(outfilefd, "LHEL!0", &r);
    // uchar r = 0x13;
    search_and_write_in_stub(outfilefd, "LHEL!0", &remainder);
    off_t firstloadsegmentoffset = pad_zero(outfilefd);
    ajust_phdr_size(outfilefd, 0, firstloadsegmentoffset, 0);
    
    set_fileds_offset(outfilefd, 0, SEEK_END);
    uchar *loader = malloc(loader_size);
    if (!loader){
        perror("Failed to allocate space for loader");
        exit(EXIT_FAILURE);
    }
    memcpy(loader, (void *)entry_loader, loader_size);
    write(outfilefd, loader, loader_size);
    // pad_zero(outfilefd);

    free(loader);
    // Libera bitmaps da tabela de codificação
    for (int i = 0; i < cods; i++)
        bitmapFree(codigos[i].bits);
}