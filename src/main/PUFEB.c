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

const char* PAKCER_SUFIX = ".lhel";
Elf64_Ehdr newelfEhdr = {
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
};

// Count the ocorrence of each char (byte) in a file
void count_char(int* freqVector, FILE* infile){
    uchar c;
    while(c = fgetc(infile), !feof(infile)){
        freqVector[c]++;
    }
}

// Index the bitmaps associated with the codes in a table by their char value
void organize_code(Code* codigos, uint cods, bitmap* codigosVet[256]) {
    for(int i = 0; i < cods; i++) {
        Code* codigo = codigos + i;
        codigosVet[codigo->c] = &codigo->bits;
    }
}

/**
 * Utiliza uma tabela de códigos `codigosVet` para codificar
 * o arquivo de `entrada`, escrevendo em `saida`
 * Retorna  Número de bits no último byte da codificação
 */
uint codificaArquivo(FILE* entrada, FILE* saida, bitmap* codigosVet[256], uint cods){
    // Armazena o byte a ser escrito na saída
    Bits outbits = {0,'\0'};

    // Enquando ainda há caracteres para ler
    uchar ch;
    while(ch = fgetc(entrada), !feof(entrada)){

        // Busca o código correspondente ao caractere
        bitmap* chbits = codigosVet[ch];  // pegaCodigo(codigos, cods, ch);
        if (!chbits) {
            fprintf(stderr, "AVISO: código inexistente\n");
            exit(1);
        }

        // Adiciona cada bit ao byte de saída
        uint tam = bitmapGetLength(*chbits);
        for(int i = 0; i < tam; i++) {
            ADD_BIT(outbits, bitmapGetBit(*chbits, i));

            // Quando preenche 8 bits
            if (outbits.pos == 8){
                // Escreve na saída
                fputc(outbits.data, saida);
                // Reseta o byte de saída
                ZERA_BITS(outbits);
            }
        }
    }
    // Se ainda sobraram bits sem completar um byte
    if (outbits.pos) {
        // Escreve o byte
        fputc(outbits.data, saida);
        // e retorna o número de bits significativos no último byte
        return outbits.pos;
    }

    // Se não sobraram bits
    return 8;
}

int main(int argc, char **argv){
    if (argc < 2) {
        printf("Usage: %s <target file> \n", *argv);
        exit(EXIT_FAILURE);
    }
    const char *base;
    PELF elf;

    // char *INPUT_FILE = argv[1];
    // char *OUTPUT_FILE;
    // char *_out_name = NULL;
    // _out_name = ALOCA_STR(strlen(INPUT_FILE) + strlen(PAKCER_SUFIX));
    // strcpy(_out_name, INPUT_FILE);
    // strcat(_out_name, PAKCER_SUFIX);
    // OUTPUT_FILE = _out_name;

    // FILE* infile = fopen(INPUT_FILE, "r");
    // if (!infile) {
    //     fprintf(stderr, "Failed to open '%s' for reading\n", INPUT_FILE);
    //     perror("fopen error: ");
    //     return 1;
    // }
    // FILE* outfile = fopen(OUTPUT_FILE, "w");
    // if (!outfile) {
    //     fprintf(stderr, "Failed to open '%s' for writing\n", OUTPUT_FILE);
    //     perror("fopen error: ");
    //     return 1;
    // }
    // free(_out_name);

    // // Sweeps the file to get the frequency of each char
    // int freq[256] = {0};
    // count_char(freq, infile);
    // rewind(infile);

    // printaFreqs(stderr, freq);

    // // Cria a lista de folhas a partir da tabela de frequências
    // ListaArv* list = Lista_cria();
    // for (int i = 0; i < 256; i++) {
    //     int fq = freq[i];
    //     if (fq){
    //         list = Lista_insOrd(list, criaFolha((uchar)i, fq));
    //     }
    // }
    // Lista_imprime(stderr, list); fprintf(stderr, "\n");

    // // Obtém a árvore ótima e libera a lista (agora vazia)
    // Arv* huff = constroiArvoreOtima(list);
    // Lista_libera(list);

    // Arv_imprime(stderr, huff); fprintf(stderr, "\n\n");

    // // Varre a árvore para criar a tabela de códigos
    // Code codigos[256]; uint cods = 0;
    // Arv_criaCodigos(huff, codigos, &cods);

    // // // Imprime ćodigos em ordem alfabética
    // qsort(codigos, cods, sizeof(Code), _compCodsAlf);
    // printaCodigos(stderr, codigos, cods); fprintf(stderr, "\n");    /// DEBUG

    // // Coloca em ordem de menor quantidade de bits (maior frequência)
    // qsort(codigos, cods, sizeof(Code), _compCodsFreq);
    // printaCodigos(stderr, codigos, cods); fprintf(stderr, "\n");

    // // Coloca os bitmaps dos códigos em um vetor associando pelas posições
    // bitmap* codigosVet[256] = {NULL};
    // organize_code(codigos, cods, codigosVet);

    // // Reserva 1 byte no início do arquivo
    // fputc('\0', outfile);

    // // Codifica uma árvore para um bitmap
    // bitmap arvSer = Arv_serializa(huff);
    // printaBitmap(stderr, arvSer); fprintf(stderr, "\n\n");

    // // Escreve a árvore codificada no arquivo
    // {
    //     uchar* arvData = bitmapGetContents(arvSer);
    //     int k = (bitmapGetLength(arvSer)+7)/8;
    //     for (int i = 0; i < k; i++)
    //         fputc(arvData[i], outfile);
    // }

    // Arv_libera(huff);
    // bitmapFree(arvSer);

    // // Codifica o arquivo de entrada, escrevendo
    // uint resto = codificaArquivo(infile, outfile, codigosVet, cods);

    // // fprintf(stderr, "resto: %u\n", resto);  /// DEBUG

    // // Escreve a quantidade de bits do último byte no primeiro byte do arquivo
    // rewind(outfile);
    // fputc((uchar)resto, outfile);


    // fclose(infile);
    // fclose(outfile);

    base = map_to_mem(argv[1]);
    elf = map_elf(base);

    // int a = insert_section(elf);

    int	fd;

    if ((fd = open("packed.lhel", O_CREAT | O_WRONLY, 0744)) < 0) {
        perror("open");
        return 1;
    }
    dump_ehdr(fd, newelfEhdr);

    close(fd);
    umap_file((char *)base, filesize);
    free_elf(elf);
    // Lista_libera(list);
    // bitmapFree(arvSer);
    // for (int i = 0; i < cods; i++)
    //     bitmapFree(codigos[i].bits);

    return 0;
}