
#include "arvore.h"

#include <stdio.h>
#include <stdlib.h>

#include "estruturas.h"
#include "util.h"


struct arv {
    uchar c;
    int freq;
    Arv* esq;
    Arv* dir;
    Arv* prox;
};

struct listaArv {
    Arv* prim;
};


Arv* criaFolha(uchar c, int freq){
    Arv* nova = (Arv*)malloc(sizeof(Arv));
    nova->c = c;
    nova->freq = freq;
    nova->esq = NULL;
    nova->dir = NULL;
    nova->prox = NULL;
    return nova;
}

Arv* criaRam(Arv* arvEsq, Arv* arvDir){
    Arv* nova = (Arv*)malloc(sizeof(Arv));
    nova->c = '\0';
    nova->freq = (arvEsq->freq) + (arvDir->freq);
    nova->esq = arvEsq;
    nova->dir = arvDir;
    nova->prox = NULL;
    return nova;
}
 
Arv* Arv_libera(Arv* arv){
    if(arv != NULL) {
        Arv_libera(arv->esq);
        Arv_libera(arv->dir);
        free(arv);
    }
    return NULL;
}


int Arv_ehFolha(Arv* arv) {
    return arv->esq == NULL;
}

uchar Arv_char(Arv* arv) {
    return arv->c;
}

int Arv_freq(Arv* arv) {
    return arv->freq;
}



// Impressão da Árvore

const char* AV_PT_IDNT = "|   ";

void _imprime (FILE* arq, Arv* arv, int nv){
    if (!arv)
        return;

    // Folha
    if (!arv->esq) {
        for(int i = 0; i < nv; i++)
            fprintf(arq, "%s", AV_PT_IDNT);
        printaCharInt(arq, arv->c);
        fprintf(arq, "\n");
    }
    // Ramificação
    else {
        for(int i = 0; i < nv; i++)
            fprintf(arq, "%s", AV_PT_IDNT);
        fprintf(arq, "<%d \n", arv->freq);

        _imprime(arq, arv->esq, nv+1);
        _imprime(arq, arv->dir, nv+1);

        for(int i = 0; i < nv; i++)
            fprintf(arq, "%s", AV_PT_IDNT);
        fprintf(arq, ">\n");
    }
}

void Arv_imprime (FILE *arq, Arv* arv){
    _imprime(arq, arv, 0);
}



// Serialização da Árvore

bitmap _criaBits(uchar* cam, uint tam) {
    bitmap bits = bitmapInit(tam);
    for(int i = 0; i < tam; i++) {
        bitmapAppendLeastSignificantBit(&bits, cam[i]);
    }
    return bits;
}

void _criaCodigos(Arv *arv, Code *cods, uint *n, uchar* cam, uint nivel) {
    if (!arv)
        return;

    // Folha
    if (!arv->esq) {
        cods[*n].c = arv->c;
        // cods[*n].freq = arv->freq;
        cods[*n].bits = _criaBits(cam, nivel);
        (*n)++;
        return;
    }

    // Ramificação
    cam[nivel] = 0;
    _criaCodigos(arv->esq, cods, n, cam, nivel+1);

    cam[nivel] = 1;
    _criaCodigos(arv->dir, cods, n, cam, nivel+1);
}

void Arv_criaCodigos(Arv *arv, Code cods[256], uint *n) {
    uchar camAtual[256];

    if (Arv_ehFolha(arv)) {
        bitmap bits = bitmapInit(1);
        bitmapAppendLeastSignificantBit(&bits, 0);

        cods[*n].c = arv->c;
        cods[*n].bits = bits;
        (*n)++;
    }
    else {
        _criaCodigos(arv, cods, n, camAtual, 0);
    }
}



void _escreveByte(bitmap* bm, uchar chr) {
    for (int i = 0; i < 8; i++) {
        uchar bit = (chr >> (7-i)) & 0x01;
        bitmapAppendLeastSignificantBit(bm, bit);
    }
}

void _serializa(Arv* arv, bitmap* bm) {
    if (!arv)
        return;

    // Folha
    if (!arv->esq) {
        bitmapAppendLeastSignificantBit(bm, 0);
        _escreveByte(bm, arv->c);
    }

    // Ramificação
    else {
        bitmapAppendLeastSignificantBit(bm, 1);
        _serializa(arv->esq, bm);
        _serializa(arv->dir, bm);
    }
}

bitmap Arv_serializa(Arv* arv) {
    bitmap bm = bitmapInit(10*256); //memória alocada supondo a pior árvore possível
    _serializa(arv, &bm);
    return bm;
}


// Desserialização

uchar _pegaProxBit(Bits* buf, FILE* arq, int* lendo){
    // Pega o próximo bit e incrementa o contador de bits
    uchar ret = PEGA_BIT(*buf, buf->pos++);

    // Caso já tenham sido lidos 8 bits, carrega novo byte do arquivo
    if (buf->pos == 8) {
        buf->data = fgetc(arq);
        buf->pos = 0;

        // Caso fim do arquivo, indica para parar o loop de leitura
        if (feof(arq))
            *lendo = 0;
    }

    return ret;
}

Arv* Arv_desserializa(FILE* arq) {
    // Armazena o caminho percorrido até o momento
    Arv* caminho[256] = {NULL};
    int nv = 0;

    // Armazena um caractere ascii
    Bits letra = (Bits){0, '\0'};
    
    // Armazena o último byte lido
    Bits buf = (Bits){0,'\0'};

    // Lê o primeiro byte e para caso EOF
    buf.data = fgetc(arq);
    if (feof(arq))
        return NULL;

    int lendo = 1; // flag setada para 0 quando encontra EOF
    do {
        uchar bit = _pegaProxBit(&buf, arq, &lendo);

        // Caso ramificação
        if (bit == 1) {
            // Incrementa o nível atual
            nv++;
        }

        // Caso folha
        else {
            // Lê o caractere
            for (int i = 0; i < 8 && lendo; i++)
                ADD_BIT(letra, _pegaProxBit(&buf, arq, &lendo));
            if (letra.pos < 8) break;

            // Cria folha e limpa o caractere
            Arv* folha = criaFolha(letra.data, 1);
            ZERA_BITS(letra);

            Arv* nova = folha;      // Árvore nova
            Arv* ant = caminho[nv]; // Árvore anterior (esquerda) (pode não existir)

            // Tenta armazenar a nova árvore no nível atual
            // e enquanto já existir uma árvore (esquerda) no nível
            while (nv && ant) {
                // combina as duas árvores
                nova = criaRam(ant, nova);
                // limpa o nível
                caminho[nv] = NULL;
                // decrementa o nível atual
                ant = caminho[--nv];
            }
            // Quando o nível atual for vazio, armazena a árvore no nível atual
            caminho[nv] = nova;
        }

    } while (nv && lendo);

    // Caso o loop tenha sido interrompido antes de zerar o nível
    if (nv) {
        fprintf(stderr, "Erro ao desserializar árvore\n");
        exit(1);
    }

    // O caso de uma árvore terminar em um byte inteiro nunca acontece
    // pois árvores binárias tem número ímpar de nós

    return caminho[0];
}


// A `ehRam` é utilizada para informar se o nó retornado é folha(0) ou ramificação(1)
Arv* anda(Arv* arv, uchar bit, int* ehRam){
    Arv* lado;
    if(bit == 0) 
        lado = arv->esq;
    else 
        lado = arv->dir;
    *ehRam = (lado->esq != NULL);
    return lado;
}



// Lista de Árvores

ListaArv* Lista_cria(){
    ListaArv* l = (ListaArv*)malloc(sizeof(ListaArv));
    l->prim = NULL;
    return l;
}

ListaArv* Lista_libera(ListaArv* lista){
    if (!lista)
        return NULL;

    Arv *p = lista->prim;
    Arv *prox;
    while(p) {
        prox = p->prox;
        free(p);
        p = prox;
    }

    free(lista);
    return NULL;
}

ListaArv* Lista_insOrd(ListaArv* lista, Arv* arv){
    if (!lista)
        return NULL;

    if(lista->prim == NULL){
        lista->prim = arv;
        return lista;
    }

    if (arv->freq < lista->prim->freq){
        arv->prox = lista->prim;
        lista->prim = arv;
        return lista;
    }

    Arv* ant = lista->prim;
    Arv* p = ant->prox;
    while (p != NULL && (p->freq < arv->freq)){
        ant = p;
        p = p->prox;
    }

    ant->prox = arv;
    arv->prox = p;

    return lista;
}

Arv* Lista_pop(ListaArv* lista){
    if(lista == NULL) return NULL;
    if(lista->prim == NULL) return NULL;
    Arv* p = lista->prim;
    lista->prim = p->prox;
    return p;
}

void Lista_imprime(FILE* saida, ListaArv* lista) {
    Arv* p = lista->prim;
    while (p != NULL) {
        fprintf(saida, "%3d : %4d\n", (int)p->c, p->freq);
        p = p->prox;
    }
}



// Algoritmo de Huffman

Arv* constroiArvoreOtima(ListaArv* lista){
    if(!lista) return NULL;
    Arv* p = lista->prim;
    if(p == NULL) return NULL;
    while(p->prox != NULL){
        Arv* a1 = Lista_pop(lista);
        Arv* a2 = Lista_pop(lista);
        Arv* novo = criaRam(a1, a2);
        Lista_insOrd(lista, novo);
        p = lista->prim;
    }
    return Lista_pop(lista);
}
