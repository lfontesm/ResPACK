#include <stdio.h>
#include <stdlib.h>

#include "estruturas.h"

/**
 * TAD de Árvore de Huffman
 *
 * Uma árvore binária em que as folhas armazenam caracteres e cada nó possui um
 * valor de frequência correspondente à soma de todos seus filhos, ou à
 * frequência de seu caractere correspondente, no caso da folha.
 * 
 * Este módulo ainda define um TAD de lista de Árvores
 */


typedef unsigned int uint;
typedef unsigned char uchar;

/// Tipo de um Árvore
typedef struct arv Arv;

/// Tipo de uma lista de Árvores
typedef struct listaArv ListaArv;


/**
 * Cria uma folha para um caractere com dada frequência
 * Entrada:
 *  c    -> o caractere
 *  freq -> a sua frequência
 * Retorna:   a nova folha
 * Pre: nenhuma
 * Pos: a folha existe alocada na heap
 */
Arv* criaFolha(uchar c, int freq);

/**
 * Cria uma ramificação (nó interno) dadas as subárvores esquerda e direita
 * Entrada: 
 *  arvEsq, arvDir -> subárvore esquerda e subárvore direita, respectivamente
 * Retorna:   a nova árvore
 * Pre: as Árvores arvEsq e arvDir existem
 * Pos: a nova Árvore existe alocada na heap, apontando para a árvores passadas;
 *      a frequência de sua raiz é a soma das frequências das subárvores
 */
Arv* criaRam(Arv* arvEsq, Arv* arvDir);

/**
 * Libera uma Árvore
 * Entrada: 
 *  arv -> a árvore a ser liberada
 * Retorna: NULL
 * Pre: a Árvore `arv` existe
 * Pos: todos os nós da Árvore `arv` foram liberados da memória
 */
Arv* Arv_libera(Arv* arv);


/**
 * Diz se o nó é uma folha
 * Entrada: arv -> uma árvore
 * Retorna: 1 se for uma folha, 0 se for ramificação
 * Pre: `arv` existe
 */
int Arv_ehFolha(Arv* arv);

/**
 * Pega o caractere de uma folha
 * Entrada: arv -> uma folha
 * Retorna: o caractere correspontende da folha
 * Pre: `arv` existe e é uma folha
 */
uchar Arv_char(Arv* arv);

/**
 * Pega a frequência de um nó
 * Entrada: arv -> uma árvore (folha ou ramificação)
 * Retorna: a frequência associada ao nó raiz
 * Pre: `arv` existe
 */
int Arv_freq(Arv* arv);


/**
 * Imprime a árvore de uma maneira hierárquica lindinha
 * Entrada:
 *  arq -> arquivo de saída onde imprimir
 *  arv -> a árvore a ser impresso\
 * Pre: `arq` é um stream aberto para escrita;
 *      `arv` existe
 */
void Arv_imprime (FILE *arq, Arv* arv);


/**
 * Percorre a árvore gerando os códigos para cada caractere em Bitmaps e os
 * armazena em um vetor  
 * Entrada:  
 *  arv -> Árvore a ser percorrida;  
 *  cods -> vetor onde serão armazenados os códigos;  
 *  n    -> ponteiro para inteiro onde armazenar o número de códigos gerado
 *          (tamanho da lista);  
 * Pre: `arv` existe;  
 *      `cods` está vazio (*n é zero);  
 * Pos: `cods` contém os códigos de todos os caracteres da Árvore;  
 *      `n` contém o número de códigos criados no vetor;  
 */
void Arv_criaCodigos(Arv *arv, Code cods[256], uint *n);


/**
 * Serializa toda a árvore seguindo a lógica de bit 1 para ramificação e bit 0
 * para folha
 * Entrada: 
 *  arv -> Árvore a ser serializada;
 * Retorna: bitmap com a codificação da árvore;
 * Pre: arvore não nula;
 * Pos: o bitmap de retorno existe;
 */
bitmap Arv_serializa(Arv* arv);

/**
 * Percorre o arquivo e traduz os binários em uma árvore necessária para
 * descodificação do arquivo
 * Entrada:
 *  arq -> arquivo compactado;
 * Retorna: a árvore desserializada a partir do arquivo;
 * Pre: 'arq' aberto para leitura;
 *      cabeçalho de 'arq' contém a árvore serializada;
 * Pos: arvore desserializada e arquivo compactado não modificado;
 */
Arv* Arv_desserializa(FILE* arq);

/**
 * Percorre (para a esquerda ou para a direita) a árvore de acordo com o 
 * bit analisado e informa se o nó retornado é folha ou ramificação
 * Entrada:
 *  arv -> arvore utilizada para descodificação do arquivo;
 *  bit -> bit analisado;
 *  ehRam -> flag utilizada para informar se o nó retornado é folha 
 *  ou ramificação;
 * Retorna: nó da árvore após ser percorrida
 * Pre: 'arv' existe;
 * Pos: 'arv' não modificada;
 *      'ehRam' é 0 caso nó retornado seja folha e 1 caso seja ramificação;
 */
Arv* anda(Arv* arv, uchar bit, int* ehRam);



/**
 * Cria uma lista de Árvores vazia  
 * Retorna: a lista  
 * Pos: a lista existe alocada na heap  
 */
ListaArv* Lista_cria();

/**
 * Libera uma lista de Árvores, incluindo todas as Árvores contidas nela
 * Entrada: lista -> a lista a ser liberada
 * Retorna: NULL
 * Pos: a lista e todos seus itens foram liberados da memória
 */
ListaArv* Lista_libera(ListaArv* lista);

/**
 * Insere uma Árvore ordenadamente (por frequencia) na lista de Árvores  
 * Entrada: 
 * - lista -> lista onde inserir a árvore;  
 * - arv -> árvore a ser inserida;  
 * Pre: 
 * - `lista` e `arv` existem;  
 * - `lista` está ordenada, o que é garantido se todos itens foram inseridos
 *   através desta função;  
 * Pos: a Árvore foi inserida na lista de modo a mantê-la ordenada de maneira
 *      crescente pelos pesos (frequências)
 */
ListaArv* Lista_insOrd(ListaArv* lista, Arv* arv);

/**
 * Retira e retorna o primeiro item (Árvore) da lista  
 * Entrada: lista -> lista de onde retirar o item
 * Retorna: o primeiro item, que foi retirado da lista; NULL se não existe
 * Pre: a `lista` existe
 * Pos: o primeiro item foi retirado da lista
 */
Arv* Lista_pop(ListaArv* lista);

/**
 * Imprime a lista de Árvores (lista dos valores dos caracteres e frequencias)  
 * Entrada: lista -> a lista
 * Retorna: nada
 * Pre: a lista existe
 * Pos: nope
 */
void Lista_imprime(FILE* saida, ListaArv* lista);


/**
 * Constrói uma Árvore de Huffman a partir de uma lista de folhas  
 * Entrada: lista -> lista de folhas que comporão a árvore  
 * Retorna: a árvore de Huffman  
 * Pre: a lista existe e tem pelo menos um item  
 * Pos: a árvore retornada existe e é a árvore ótima de codificação;  
 *      `lista` está vazia;  
 */
Arv* constroiArvoreOtima(ListaArv* lista);

