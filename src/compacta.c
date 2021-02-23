#include "compacta.h"

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

// Coloca os bitmaps referentes aos cógigos em uma tabela (vetor) indexado
// pelo valor dos caracteres
void organizaCodigos (Code* codigos, uint cods, bitmap* codigosVet[256]) {
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
Elf64_Off segmentsize;
uint codificaArquivo(int intputfilefd, int outfilefd, bitmap* codigosVet[256], uint cods){
    // Armazena o byte a ser escrito na saída
    Bits outbits = {0,'\0'};

    // Enquando ainda há caracteres para ler
    uchar ch;
    ssize_t nread, nwrite;

    while (nread = read(intputfilefd, &ch, sizeof(ch)), nread > 0) {
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
                // fputc(outbits.data, saida);
                segmentsize += nwrite = write(outfilefd, &outbits.data, sizeof(outbits.data));
                if (nwrite == -1){
                    perror("Failed to write encoded file");
                    exit(EXIT_FAILURE);
                }
                // Reseta o byte de saída
                ZERA_BITS(outbits);
            }
        }
    }



    // Se ainda sobraram bits sem completar um byte
    if (outbits.pos) {
        // Escreve o byte
        // fputc(outbits.data, saida);
        segmentsize += nwrite = write(outfilefd, &outbits.data, sizeof(outbits.data));
        if (nwrite == -1){
            perror("Failed to write encoded file");
            exit(EXIT_FAILURE);
        }
        // pad_zero(outfilefd);
        // e retorna o número de bits significativos no último byte
        return outbits.pos;
    }

    // Se não sobraram bits
    return 8;
}


// const char* ARQ_ENTRADA = "teste/entrada.txt";
// const char* ARQ_SAIDA = "teste/saida.bin";

// int main(int argv, char** argc) {

//     if (argv <= 1) { /* garante um parâmetro */
//         fprintf(stderr, "Uso: %s <entrada> [saida]\n", argc[0]);
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
//             ALOCA_STR( strlen(ARQ_ENTRADA) + strlen(SUFIXO_COMP) );
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

//     // printaFreqs(stderr, freq);    /// DEBUG

//     // Cria a lista de folhas a partir da tabela de frequências
//     ListaArv* lista = Lista_cria();
//     for (int i = 0; i < 256; i++) {
//         int fq = freq[i];
//         if (fq){
//             lista = Lista_insOrd(lista, criaFolha((uchar)i, fq));
//         }
//     }

//     // Lista_imprime(stderr, lista); fprintf(stderr, "\n");  /// DEBUG

//     // Obtém a árvore ótima e libera a lista (agora vazia)
//     Arv* huff = constroiArvoreOtima(lista);
//     Lista_libera(lista);

//     // Arv_imprime(stderr, huff); fprintf(stderr, "\n\n");   /// DEBUG

//     // Varre a árvore para criar a tabela de códigos
//     Code codigos[256]; uint cods = 0;
//     Arv_criaCodigos(huff, codigos, &cods);

//     // // Imprime ćodigos em ordem alfabética
//     // qsort(codigos, cods, sizeof(Code), _compCodsAlf);
//     // printaCodigos(stderr, codigos, cods); fprintf(stderr, "\n");    /// DEBUG

//     // // Coloca em ordem de menor quantidade de bits (maior frequência)
//     // qsort(codigos, cods, sizeof(Code), _compCodsFreq);
//     // printaCodigos(stderr, codigos, cods); fprintf(stderr, "\n");    /// DEBUG


//     // Coloca os bitmaps dos códigos em um vetor associando pelas posições
//     bitmap* codigosVet[256] = {NULL};
//     organizaCodigos(codigos, cods, codigosVet);


//     // Reserva 1 byte no início do arquivo
//     fputc('\0', arqSaida);

//     // Codifica uma árvore para um bitmap
//     bitmap arvSer = Arv_serializa(huff);
//     // printaBitmap(stderr, arvSer); fprintf(stderr, "\n\n");    /// DEBUG

//     // ? Passar último byte (buffer) para a parte de codificação do texto?

//     // Escreve a árvore codificada no arquivo
//     {
//         uchar* arvData = bitmapGetContents(arvSer);
//         int k = (bitmapGetLength(arvSer)+7)/8;
//         for (int i = 0; i < k; i++)
//             fputc(arvData[i], arqSaida);
//     }

//     Arv_libera(huff);
//     bitmapFree(arvSer);

//     // Codifica o arquivo de entrada, escrevendo
//     uint resto = codificaArquivo(arqEntrada, arqSaida, codigosVet, cods);

//     // fprintf(stderr, "resto: %u\n", resto);  /// DEBUG

//     // Escreve a quantidade de bits do último byte no primeiro byte do arquivo
//     rewind(arqSaida);
//     fputc((uchar)resto, arqSaida);

//     // Libera bitmaps da tabela de codificação
//     for (int i = 0; i < cods; i++)
//         bitmapFree(codigos[i].bits);

//     if (_nome_saida) free(_nome_saida);
//     fclose(arqEntrada);
//     fclose(arqSaida);

//     return 0;
// }
