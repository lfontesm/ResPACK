#include <stdio.h>
#include <stdlib.h>

#include "structures.h"

/**
 * Huffman tree ADT
 *
 * A binary tree in which the leafs store characters and each node has
 * a frequency value corresponding to the sum of all of its child nodes,
 * or the frequency of it's corresponding character, in case of a leaf
 * 
 * This module also defines an ADT for the list of trees
 */


typedef unsigned int uint;
typedef unsigned char uchar;

/// Tree type
typedef struct tree Tree;

/// Tree list type
typedef struct listaArv TreeList;


/**
 * Creates a leaf for a char with a given frequency
 @param  c    -> char
 @param  freq -> frequency
 @return: a new leaf
 @pre: none
 @post: leaf is allocated in heap
 */
Tree* leaf_create(uchar c, int freq);

/**
 * Creates a branch (inside node) given the subtress left and right
 @param  leftTree  -> subtrees left and right
 @param  rightTree
 @return: New tree
 @pre: leftTree and rightTree exist
 @post: New treee is allocated in heap
 */
Tree* create_branch(Tree* leftTree, Tree* rightTree);

/**
 * Frees a tree
 @param  tree -> tree to e freed
 @return: none
 @pre: tree exists
 @post: all nodes from tree are freed
 */
Tree* free_tree(Tree* tree);


/**
 * Checks if node is tree
 @param tree -> tree
 @return: 1 if leaf, 0 otherwise
 @pre: `tree` exists
 */
int tree_is_leaf(Tree* tree);

/**
 * Gets the char from a leaf
 @param tree -> a leaf
 @return: the char from the leaf
 @pre: `tree` exists and is a leaf
 */
uchar tree_char(Tree* tree);

/**
 * Gets the frequency of a node
 @param tree -> a tree (leaf ou branch)
 @return: frequency of the node
 @pre: `tree` exists
 */
int tree_freq(Tree* tree);


/**
 * Prints a tree in an hierarchical manner
 @param  outfile -> output file
 @param  tree    -> tree to be printed
 @pre: `outfile` is an opened strem to writing;
 *      and `tree` exists
 */
void print_tree (FILE *outfile, Tree* tree);


/**
 * Walks through the tree generating codes for each char in bitmaps and stores them in a vector
 @param  tree -> tree to be walked  
 @param  cods -> vector where the codes will be stored
 @param  n    -> pointer to integer where it will store the number of codes generated
 @pre: `tree` existes  
 *      `cods` is empty (*n is 0);  
 @post: `cods` contains the codes from all chars in tree
 *      `n` contains the number of codes created in vector
 */
void tree_create_codes(Tree *tree, Code cods[256], uint *n);


/**
 * Serialize the tree, following the logic of 1 bit for branch and 0 for leaf
 @param  tree -> tree to be serialized;
 @return: bitmap with the codes from the tree
 @pre: tree is not null;
 @post: bitmap existis and is returned
 */
bitmap tree_serialize(Tree* tree);

/**
 * Walks the file and translate the binaries into a tree necessary for deserializing the file
 @param outfile -> packed file;
 @return: deserialized tree from file;
 @pre: 'outfile' opened for reading;
 *      header of outfile contains a serialized tree
 @post: deserialized tree and packed file not modified
 */
Tree* tree_deserialize(FILE* outfile);

/**
 * Walks the tree according to the analised bit and tells if returned nod is leaf or branch
 @param tree -> tree used for deserealizing the file;
 @param bit -> analyzed bit;
 @param isBranch -> flag used to tell if returned node is leaf or branch
 @return: tree node after being walkedthrough
 @pre: 'tree' exists;
 @post: 'tree' is not modified;
 *      'isBranch' is 0 in case of leaf, 1 otherwise
 */
Tree* walk(Tree* tree, uchar bit, int* isBranch);



/**
 * Creates a list of empty trees
 @return: a list
 @post: list exists in heap
 */
TreeList* create_list();

/**
 * Free a list of trees, including all the tress in it
 @param list -> list to be freed
 @return: NULL
 @post: the list and all its itens are freed
 */
TreeList* free_list(TreeList* list);

/**
 * Inserts a tree sorted by frequency in a list of tree
 @param list -> list where tree will be inserted;
 @param tree -> tree to be inserted;
 @pre: 
 * - `list` and `tree` exist;
 * - `list` is ordered
 @post: tree was inserted and list remains ordered
 */
TreeList* list_insOrd(TreeList* list, Tree* tree);

/**
 * Pops the first item from list
 @param list -> list to pop
 @return: first item from list, NULL if is empty
 @pre: `list` exists
 @post: first item is not in the list anymore
 */
Tree* list_pop(TreeList* list);

/**
 * Prints the list of trees
 @param list -> list
 @return: none
 @pre: list exists
 @post: :O
 */
void list_print(FILE* outFile, TreeList* list);


/**
 * Builds the Huffman Tree based on a list of leafs
 @param list -> list of leafs that will compose the tree
 @return: the huffman tree
 @pre: list exists and has at least an item
 @post: the returned tree exists and is the optimal tree. List is empty
 */
Tree* build_optimal_tree(TreeList* list);

