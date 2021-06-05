
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

#include "structures.h"
#include "util.h"


struct tree {
    uchar c;
    int freq;
    Tree* left;
    Tree* right;
    Tree* next;
};

struct listaArv {
    Tree* first;
};


Tree* leaf_create(uchar c, int freq){
    Tree* new = (Tree*)malloc(sizeof(Tree));
    new->c = c;
    new->freq = freq;
    new->left = NULL;
    new->right = NULL;
    new->next = NULL;
    return new;
}

Tree* create_branch(Tree* leftTree, Tree* rightTree){
    Tree* new = (Tree*)malloc(sizeof(Tree));
    new->c = '\0';
    new->freq = (leftTree->freq) + (rightTree->freq);
    new->left = leftTree;
    new->right = rightTree;
    new->next = NULL;
    return new;
}
 
Tree* free_tree(Tree* tree){
    if(tree != NULL) {
        free_tree(tree->left);
        free_tree(tree->right);
        free(tree);
    }
    return NULL;
}


int tree_is_leaf(Tree* tree) {
    return tree->left == NULL;
}

uchar tree_char(Tree* tree) {
    return tree->c;
}

int tree_freq(Tree* tree) {
    return tree->freq;
}



// Print tree

const char* AV_PT_IDNT = "|   ";

void _print (FILE* arq, Tree* tree, int height){
    if (!tree)
        return;

    // Leaf
    if (!tree->left) {
        for(int i = 0; i < height; i++)
            fprintf(arq, "%s", AV_PT_IDNT);
        print_int_char(arq, tree->c);
        fprintf(arq, "\n");
    }
    // branch
    else {
        for(int i = 0; i < height; i++)
            fprintf(arq, "%s", AV_PT_IDNT);
        fprintf(arq, "<%d \n", tree->freq);

        _print(arq, tree->left, height+1);
        _print(arq, tree->right, height+1);

        for(int i = 0; i < height; i++)
            fprintf(arq, "%s", AV_PT_IDNT);
        fprintf(arq, ">\n");
    }
}

void print_tree (FILE *arq, Tree* tree){
    _print(arq, tree, 0);
}



// Serialize tree

bitmap _create_bits(uchar* cam, uint tam) {
    bitmap bits = bitmapInit(tam);
    for(int i = 0; i < tam; i++) {
        bitmapAppendLeastSignificantBit(&bits, cam[i]);
    }
    return bits;
}

void _create_codes(Tree *tree, Code *cods, uint *n, uchar* cam, uint height) {
    if (!tree)
        return;

    // Leaf
    if (!tree->left) {
        cods[*n].c = tree->c;
        // cods[*n].freq = tree->freq;
        cods[*n].bits = _create_bits(cam, height);
        (*n)++;
        return;
    }

    // Branch
    cam[height] = 0;
    _create_codes(tree->left, cods, n, cam, height+1);

    cam[height] = 1;
    _create_codes(tree->right, cods, n, cam, height+1);
}

void tree_create_codes(Tree *tree, Code cods[256], uint *n) {
    uchar currentCam[256];

    if (tree_is_leaf(tree)) {
        bitmap bits = bitmapInit(1);
        bitmapAppendLeastSignificantBit(&bits, 0);

        cods[*n].c = tree->c;
        cods[*n].bits = bits;
        (*n)++;
    }
    else {
        _create_codes(tree, cods, n, currentCam, 0);
    }
}



void _write_byte(bitmap* bm, uchar chr) {
    for (int i = 0; i < 8; i++) {
        uchar bit = (chr >> (7-i)) & 0x01;
        bitmapAppendLeastSignificantBit(bm, bit);
    }
}

void _serialize(Tree* tree, bitmap* bm) {
    if (!tree)
        return;

    // Leaf
    if (!tree->left) {
        bitmapAppendLeastSignificantBit(bm, 0);
        _write_byte(bm, tree->c);
    }

    // Branch
    else {
        bitmapAppendLeastSignificantBit(bm, 1);
        _serialize(tree->left, bm);
        _serialize(tree->right, bm);
    }
}

bitmap tree_serialize(Tree* tree) {
    bitmap bm = bitmapInit(10*256); // allocate memory for the worst case scenario
    _serialize(tree, &bm);
    return bm;
}


// Deserialize

uchar _get_next_bit(Bits* buf, FILE* arq, int* reading){
    // Get next bit and increment the bit counter
    uchar ret = GET_BIT(*buf, buf->pos++);

    // If it's already read 8 bits, load another byte
    if (buf->pos == 8) {
        buf->data = fgetc(arq);
        buf->pos = 0;

        // If EOF, stop reading
        if (feof(arq))
            *reading = 0;
    }

    return ret;
}

Tree* tree_deserialize(FILE* arq) {
    // Store the path walked thus far
    Tree* path[256] = {NULL};
    int height = 0;

    // Stores an ascii char
    Bits aChar = (Bits){0, '\0'};
    
    // Stores the last byte read
    Bits buf = (Bits){0,'\0'};

    // Reads first byte and stops in case of EOF
    buf.data = fgetc(arq);
    if (feof(arq))
        return NULL;

    int reading = 1; // flag setted to 0 when EOF is found
    do {
        uchar bit = _get_next_bit(&buf, arq, &reading);

        // Branch
        if (bit == 1) {
            // Increment current height of tree
            height++;
        }

        // Leaf
        else {
            // Reads char
            for (int i = 0; i < 8 && reading; i++)
                ADD_BIT(aChar, _get_next_bit(&buf, arq, &reading));
            if (aChar.pos < 8) break;

            // Creates a leaf and cleans the char
            Tree* leaf = leaf_create(aChar.data, 1);
            ZERO_BITS(aChar);

            Tree* new = leaf;      // New tree
            Tree* prev = path[height]; // Previous tree (left) (may not exist)

            // Tries to store a new tree in current height
            // and while theres already a left tree in its height
            while (height && prev) {
                // combina as duas árvores
                new = create_branch(prev, new);
                // clears height
                path[height] = NULL;
                // decrement current height
                prev = path[--height];
            }
            // When current height is empty, store tree in it
            path[height] = new;
        }

    } while (height && reading);

    // In case loop was interrupted before clearing height
    if (height) {
        fprintf(stderr, "Error in deseralizing tree\n");
        exit(1);
    }

    // The case of a tree ending in a full byte never happens
    // because bin tress have an odd number of nodes

    return path[0];
}


// isBranch its used to tell if the returned node is leaf(0) of branch(1)
Tree* walk(Tree* tree, uchar bit, int* isBranch){
    Tree* side;
    if(bit == 0) 
        side = tree->left;
    else 
        side = tree->right;
    *isBranch = (side->left != NULL);
    return side;
}



// Tree list

TreeList* create_list(){
    TreeList* l = (TreeList*)malloc(sizeof(TreeList));
    l->first = NULL;
    return l;
}

TreeList* free_list(TreeList* list){
    if (!list)
        return NULL;

    Tree *p = list->first;
    Tree *next;
    while(p) {
        next = p->next;
        free(p);
        p = next;
    }

    free(list);
    return NULL;
}

TreeList* list_insOrd(TreeList* list, Tree* tree){
    if (!list)
        return NULL;

    if(list->first == NULL){
        list->first = tree;
        return list;
    }

    if (tree->freq < list->first->freq){
        tree->next = list->first;
        list->first = tree;
        return list;
    }

    Tree* prev = list->first;
    Tree* p = prev->next;
    while (p != NULL && (p->freq < tree->freq)){
        prev = p;
        p = p->next;
    }

    prev->next = tree;
    tree->next = p;

    return list;
}

Tree* list_pop(TreeList* list){
    if(list == NULL) return NULL;
    if(list->first == NULL) return NULL;
    Tree* p = list->first;
    list->first = p->next;
    return p;
}

void list_print(FILE* outFile, TreeList* list) {
    Tree* p = list->first;
    while (p != NULL) {
        fprintf(outFile, "%3d : %4d\n", (int)p->c, p->freq);
        p = p->next;
    }
}



// Huffman algorithm

Tree* build_optimal_tree(TreeList* list){
    if(!list) return NULL;
    Tree* p = list->first;
    if(p == NULL) return NULL;
    while(p->next != NULL){
        Tree* a1 = list_pop(list);
        Tree* a2 = list_pop(list);
        Tree* new = create_branch(a1, a2);
        list_insOrd(list, new);
        p = list->first;
    }
    return list_pop(list);
}
