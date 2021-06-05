#include "buildelfoutput.h"
#include "pack.h"
#include "basemem.h"

/* 
    This file will be responsible for building the output, e.g, the packed ELF binary
*/

extern u_int64_t        loader_size;
extern u_int64_t        infos_size;
extern void             entry_loader(void);

/* Defining the structs we will use in this file to build the new elf file */
#define MAX_PHDR_NUM 3

typedef struct {
    Elf64_Ehdr ehdr;
    Elf64_Off  offsetinfile;
} NEW_EHDR;

/* Initialize the structs */
NEW_EHDR newehdr = {
    .ehdr = {
        .e_ident     = {'\x7f','\x45','\x4c','\x46','\x02','\x01','\x01','\x03','\x00','\x00','\x00','\x00','\x00','\x00','\x00','\x00'},	/* ELF "magic number" */
        .e_type      = ET_EXEC,
        .e_machine   = 0x3E,
        .e_version   = 0x01,
        .e_entry     = 0xffffffff,  /* Entry point virtual address */
        .e_phoff     = 64,	        /* Program header table file offset */
        .e_shoff	 = 0,           /* Section header table file offset */
        .e_flags     = 0,
        .e_ehsize    = 64,
        .e_phentsize = 56,
        .e_phnum     = 1,
        .e_shentsize = 64,
        .e_shnum     = 0,
        .e_shstrndx  = 0
    },
    .offsetinfile = 0
};

Elf64_Phdr phdr__ = {
    .p_type   = PT_LOAD,
    .p_flags  = PF_X | PF_R,
    .p_offset = 0x40,
    .p_vaddr  = 0x400040,
    .p_paddr  = 0x400040,
    .p_filesz = 0xffffffff,
    .p_memsz  = 0xffffffff,
    .p_align  = 0x1000
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
    int offset = write(outfilefd, &(newehdr.ehdr), sizeof(Elf64_Ehdr));
    if (offset == -1) {
        perror("Operation write failed: ");
        exit(EXIT_FAILURE);
    }
    if (offset != sizeof(newehdr.ehdr)) 
        puts("Writing of EHDR was probably not succesful");
        
}

off_t update_entrypoint(int outfilefd, off_t newEntryPointOff){
    ssize_t nread, nwrite;
    Elf64_Ehdr ehdr;
    nread = read(outfilefd, &ehdr, sizeof(ehdr));
    if (nread != sizeof(ehdr)){
        fprintf(stderr, "Failed to read EHDR\n");
        exit(EXIT_FAILURE);
    }

    set_fileds_offset(outfilefd, 0, SEEK_SET);

    ehdr.e_entry = 0x400000 + newEntryPointOff;
    nwrite = write(outfilefd, &ehdr, sizeof(ehdr));
    if (nwrite != sizeof(ehdr)){
        fprintf(stderr, "Failed to write new entry point");
        exit(EXIT_FAILURE);
    }

    // Get the size of the LOAD segment in the file (rounded up and as a power of 2) while updating the entry point
    // used in update_phdr_size
    off_t segmentsize = newEntryPointOff;
    while ((++segmentsize % 0x1000) != 0);
    // printf("Next addr: %x\n", segmentsize);

    set_fileds_offset(outfilefd, 0, SEEK_END);

    return segmentsize;
}

/* 
    Write program header table
 */
void write_phdr(int outfilefd){
    int nwrite = write(outfilefd, &phdr__, sizeof(phdr__));
    if (nwrite < sizeof(phdr__)){
        perror("Operation write failed: ");
        exit(EXIT_FAILURE);
    }
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

/*  IDEA: In case of adding more than 1 Phdr in the future, make a wrapper function on top of this one
          instead of rewritting it
    Update the size of a header from the program header table.
    Since this file only has 1 Phdr, update that one
 */
void ajust_phdr_size(int outfilefd, int indx, Elf64_Xword size){
    set_fileds_offset(outfilefd, 0, SEEK_SET);
    
    char buf[256];
    read(outfilefd, &buf, 0x60);
    
    ssize_t nwrite = write(outfilefd, &size, 4);

    if (nwrite < 4) {
        perror("Error in writing new size in PHDR");
        exit(1);
    }
    read(outfilefd, &buf, 4);

    nwrite = write(outfilefd, &size, 4);
    if (nwrite < 4) {
        perror("Error in writing new size in PHDR");
        exit(1);
    }

    // free(stringify);

}

// extern Elf64_Off segmentsize;
// Used to get a filesize of order of 2
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
    int seekoffset;

    // Init frequency vector by reading all inputfile to get the frequency of the characters
    // and goes back to the beggining
    int freq[256] = {0};
    count_char(freq, inputfilefd);
    if ((seekoffset = lseek(inputfilefd, 0, SEEK_SET)) == -1){
        perror("Lseek failed");
        exit(EXIT_FAILURE);
    }

    // Buffer used to get the size of the input file.
    // This size will be written next to the stub.
    struct stat statbuf;
    int fstaterr = fstat(inputfilefd, &statbuf);
    if (fstaterr == -1) {
         perror("Failed to retrieve information about input file");
        exit(EXIT_FAILURE);
    }

    // print_freqs(stderr, freq);    /// DEBUG

    // Create the list of leafs from the frequency table
    TreeList* list = create_list();
    for (int i = 0; i < 256; i++) {
        int fq = freq[i];
        if (fq){
            list = list_insOrd(list, leaf_create((uchar)i, fq));
        }
    }

    // list_print(stderr, list); fprintf(stderr, "\n");  /// DEBUG

    // Builds the optimal tree and free the list
    Tree* huff = build_optimal_tree(list);
    free_list(list);

    // print_tree(stderr, huff); fprintf(stderr, "\n\n");   /// DEBUG

    // exit(1); // DEBUG

    // Reads the tree to create the code table
    Code codes[256]; uint cods = 0;
    tree_create_codes(huff, codes, &cods);

    // // Print codes in alphabetical order
    // qsort(codes, cods, sizeof(Code), _compCodsAlf);
    // print_codes(stderr, codes, cods); fprintf(stderr, "\n");    /// DEBUG

    // // Prints in order of frequency
    // qsort(codes, cods, sizeof(Code), _compCodsFreq);
    // print_codes(stderr, codes, cods); fprintf(stderr, "\n");    /// DEBUG

    // Puts the bitmaps from the codes in a table associated by their index
    bitmap* codesVec[256] = {NULL};
    organize_code(codes, cods, codesVec);

    // Stub of output file
    ssize_t stubn = write(outfilefd, "LEL0", strlen("LEL0"));
    if (stubn != strlen("LEL0")) {  
        puts("Something went wrong when writing the LEL stub");
        exit(EXIT_FAILURE);
    }
    ssize_t sizen = write(outfilefd, &statbuf.st_size, 4);
    if (sizen != 4){
        puts("Something went wrong when writing the size of original file next to sutb");
        exit(EXIT_FAILURE);
    }

    // Encode tree into a bitmap
    bitmap arvSer = tree_serialize(huff);
    // print_bitmap(stderr, arvSer); fprintf(stderr, "\n\n");    /// DEBUG

    // Write enconded tree in outputfile
    {
        uchar* arvData = bitmapGetContents(arvSer);
        int k = (bitmapGetLength(arvSer)+7)/8;
        for (int i = 0; i < k; i++)
            write(outfilefd, &arvData[i], sizeof(arvData[i]));
    }

    free_tree(huff);
    bitmapFree(arvSer);

    // Encode input file, and writes it into the output file (soon to be packed bin)
    uchar remainder = encode_file(inputfilefd, outfilefd, codesVec, cods);

    // fprintf(stderr, "resto: %u\n", resto);  /// DEBUG

    search_and_write_in_stub(outfilefd, "LEL0", &remainder);
    off_t entryPointOff = pad_zero(outfilefd);
    

    set_fileds_offset(outfilefd, 0, SEEK_END);
    uchar *loader = malloc(loader_size);
    if (!loader){
        perror("Failed to allocate space for loader");
        exit(EXIT_FAILURE);
    }

    printf("entryPointOff %lx\n", entryPointOff);
    
    memcpy(loader, (void *)entry_loader, loader_size);
    memcpy(loader + loader_size - 8, &entryPointOff, sizeof(ssize_t));
    write(outfilefd, loader, loader_size);

    set_fileds_offset(outfilefd, 0, SEEK_SET);
    
    off_t segmentesize = update_entrypoint(outfilefd, entryPointOff);
    printf("Size after entry was added %lx\n", segmentesize);


    ajust_phdr_size(outfilefd, 0, segmentesize);

    // Fress the memory allocated for the loader
    free(loader);

    // Frees the bitmaps from the encoding table
    for (int i = 0; i < cods; i++)
        bitmapFree(codes[i].bits);
}