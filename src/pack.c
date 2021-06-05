#include "pack.h"

const char* SUFIXO_COMP = ".comp";


// Count the ocorrence of each char (byte) in a file
void count_char(int* freqVet, int inputfilefd){
    uchar c;
    int nread;
    while (nread = read(inputfilefd, &c, sizeof(c)), nread > 0){
        // putchar(c);
        // puts("");
        freqVet[c]++;
    }
    // while(c = fgetc(entrada), !feof(entrada)){
    //     freqVet[c]++;
    // }
}

// Stores the bitmaps refering to the codes in a vector indexed
// by the int value of chars
void organize_code (Code* codes, uint cods, bitmap* codesVec[256]) {
    for(int i = 0; i < cods; i++) {
        Code* code = codes + i;
        codesVec[code->c] = &code->bits;
    }
}

/**
 * Use a code table `codesVec` to encode
 * the file `inputfilefd`, writing in `outfilefd`
 * Returns the number of bits in the last byte of the enconding
 */
// Elf64_Off segmentsize = (unsigned long long)64;
uint encode_file(int inputfilefd, int outfilefd, bitmap* codesVec[256], uint cods){
    // Stores the byte to be written in the output
    Bits outbits = {0,'\0'};

    // While it still remains chars to be read
    uchar ch;
    ssize_t nread, nwrite;

    while (nread = read(inputfilefd, &ch, sizeof(ch)), nread > 0) {
        // Fetch the code "pointed" by the char
        bitmap* chbits = codesVec[ch];  // pegaCodigo(codes, cods, ch);
        if (!chbits) {
            fprintf(stderr, "WARNING: nonexistent code\n");
            exit(1);
        }

        // Add each bit to ouput byte
        uint tam = bitmapGetLength(*chbits);
        for(int i = 0; i < tam; i++) {
            ADD_BIT(outbits, bitmapGetBit(*chbits, i));

            // When it reaches 8 bits
            if (outbits.pos == 8){
                // Writes in output
                // fputc(outbits.data, outFile);
                /* segmentsize +=  */nwrite = write(outfilefd, &outbits.data, sizeof(outbits.data));
                if (nwrite == -1){
                    perror("Failed to write encoded file");
                    exit(EXIT_FAILURE);
                }
                // Resets output byte
                ZERO_BITS(outbits);
            }
        }
    }



    // If it still remains bits without completing a byte
    if (outbits.pos) {
        // Writes byte
        // fputc(outbits.data, outFile);
        /* segmentsize +=  */nwrite = write(outfilefd, &outbits.data, sizeof(outbits.data));
        if (nwrite == -1){
            perror("Failed to write encoded file");
            exit(EXIT_FAILURE);
        }
        // pad_zero(outfilefd);
        // and returns the number of significant bits in last byte
        return outbits.pos;
    }

    // Se não sobraram bits
    return 8;
}


// const char* ARQ_ENTRADA = "teste/entrada.txt";
// const char* ARQ_SAIDA = "teste/outFile.bin";

// int main(int argv, char** argc) {

//     if (argv <= 1) { /* garante um parâmetro */
//         fprintf(stderr, "Uso: %s <entrada> [outFile]\n", argc[0]);
//         exit(1);
//     }

//     char* ARQ_ENTRADA = argc[1];
//     char* ARQ_SAIDA;
//     char* _nome_saida = NULL;

//     // Caso haja dois argumentos, usa o segundo como nome da saída
//     if (argv-1 >= 2) {
//         ARQ_SAIDA = argc[2];
//     }
//     // Senão, cria um nome concatenando o sufixo ".comp" ao nome de entrada
//     else {
//         _nome_saida =
//             ALLOC_STR( strlen(ARQ_ENTRADA) + strlen(SUFIXO_COMP) );
//         strcpy(_nome_saida, ARQ_ENTRADA);
//         strcat(_nome_saida, SUFIXO_COMP);
//         ARQ_SAIDA = _nome_saida;
//     }

//     FILE* arqEntrada = fopen(ARQ_ENTRADA, "r");
//     if (!arqEntrada) {
//         fprintf(stderr, "Falha ao abrir '%s' para leitura\n", ARQ_ENTRADA);
//         perror("ERRO");
//         return 1;
//     }
//     FILE* arqSaida = fopen(ARQ_SAIDA, "w");
//     if (!arqSaida) {
//         fprintf(stderr, "Falha ao abrir '%s' para escrita\n", ARQ_SAIDA);
//         perror("ERRO");
//         return 1;
//     }

//     // Varre todo o arquivo para obter frequência de cada caractere
//     // e volta para o início
//     int freq[256] = {0};
//     count_char(freq, arqEntrada);
//     rewind(arqEntrada);

//     // print_freqs(stderr, freq);    /// DEBUG

//     // Cria a list de folhas a partir da tabela de frequências
//     TreeList* list = create_list();
//     for (int i = 0; i < 256; i++) {
//         int fq = freq[i];
//         if (fq){
//             list = list_insOrd(list, leaf_create((uchar)i, fq));
//         }
//     }

//     // list_print(stderr, list); fprintf(stderr, "\n");  /// DEBUG

//     // Obtém a árvore ótima e libera a list (agora vazia)
//     Tree* huff = build_optimal_tree(list);
//     free_list(list);

//     // print_tree(stderr, huff); fprintf(stderr, "\n\n");   /// DEBUG

//     // Varre a árvore para criar a tabela de códigos
//     Code codes[256]; uint cods = 0;
//     tree_create_codes(huff, codes, &cods);

//     // // Imprime ćodigos em ordem alfabética
//     // qsort(codes, cods, sizeof(Code), _compCodsAlf);
//     // print_codes(stderr, codes, cods); fprintf(stderr, "\n");    /// DEBUG

//     // // Coloca em ordem de menor quantidade de bits (maior frequência)
//     // qsort(codes, cods, sizeof(Code), _compCodsFreq);
//     // print_codes(stderr, codes, cods); fprintf(stderr, "\n");    /// DEBUG


//     // Coloca os bitmaps dos códigos em um vetor associando pelas posições
//     bitmap* codesVec[256] = {NULL};
//     organize_code(codes, cods, codesVec);


//     // Reserva 1 byte no início do arquivo
//     fputc('\0', arqSaida);

//     // Codifica uma árvore para um bitmap
//     bitmap arvSer = tree_serialize(huff);
//     // print_bitmap(stderr, arvSer); fprintf(stderr, "\n\n");    /// DEBUG

//     // ? Passar último byte (buffer) para a parte de codificação do texto?

//     // Escreve a árvore codificada no arquivo
//     {
//         uchar* arvData = bitmapGetContents(arvSer);
//         int k = (bitmapGetLength(arvSer)+7)/8;
//         for (int i = 0; i < k; i++)
//             fputc(arvData[i], arqSaida);
//     }

//     free_tree(huff);
//     bitmapFree(arvSer);

//     // Codifica o arquivo de entrada, escrevendo
//     uint resto = encode_file(arqEntrada, arqSaida, codesVec, cods);

//     // fprintf(stderr, "resto: %u\n", resto);  /// DEBUG

//     // Escreve a quantidade de bits do último byte no primeiro byte do arquivo
//     rewind(arqSaida);
//     fputc((uchar)resto, arqSaida);

//     // Libera bitmaps da tabela de codificação
//     for (int i = 0; i < cods; i++)
//         bitmapFree(codes[i].bits);

//     if (_nome_saida) free(_nome_saida);
//     fclose(arqEntrada);
//     fclose(arqSaida);

//     return 0;
// }
