/** Defines an ADT representing a bitmap
 * @file bitmap.c
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"

/**
 * Auxiliary function that prints an error msg and abort the executions in case testresult is false.
 * @param testresult Boolean value representing the result of the test that should be true.
 * @param message A message to be printed if the result of the test is false. 
 */
void assert(int testresult, char* message) {
	if (!testresult) {
		fprintf(stderr, "%s\n", message);
		exit(EXIT_FAILURE);
	}
}


/**
 * Returns the content of the bitmap.
 * @param bm The bitmap.
 */
unsigned char* bitmapGetContents(bitmap bm) {
	return bm.contents;
}

/**
 * Returns the maximus size of the bitmap.
 * @param bm The bitmap.
 * @return The maximum size of the bitmap.
 */
unsigned int bitmapGetMaxSize(bitmap bm) {
	return bm.max_size;
}

/**
 * Returns the current size of the bitmap.
 * @param bm The bitmap.
 * @return The current size of bitmap.
 */
unsigned int bitmapGetLength(bitmap bm) {
	return bm.length;
}

/**
 * Builds a new bitmap, defining a maximum size.
 * @param max_size The maximum size for the bitmap.
 * @return The initialized bitmap.
 */
bitmap bitmapInit(unsigned int max_size) {	
	bitmap bm;
	// define maximum size in bytes, rouding up
	unsigned int max_sizeInBytes=(max_size+7)/8;
	// allocate memmory for the maximum size, in bytes
	bm.contents=calloc(max_sizeInBytes, sizeof(char));
	// verify mem allocation
	assert(bm.contents!=NULL, "Mem allocation err.");
	// define initial values for max size and curr size
	bm.max_size=max_size;
	bm.length=0;
	return bm;
}

/**
 * Return the value of bit in [index]
 * @param bm The bitmap.
 * @param index Bit position.
 * @pre index<bitmapGetLength(bm)
 * @return The bit value.
 */
unsigned char bitmapGetBit(bitmap bm, unsigned int index) // index in bits
{
	// verify if index<bm.length, because otherwise, index is valid
	assert(index<bm.length, "Invalid access in bitmap.");
	// index/8 its the index of the byte that contains the bit
	// 7-(index%8) its the dislocation of the bit in the byte
	return (bm.contents[index/8] >> (7-(index%8))) & 0x01;
}

/**
 * Modifica o valor do bit na posicao index.
 * @param bm O mapa de bits.
 * @param index A posicao do bit.
 * @param bit O novo valor do bit.
 * @post bitmapGetBit(bm,index)==bit
 */
void bitmapSetBit(bitmap* bm, unsigned int index, unsigned char bit) {
	// verify if index<bm.length, because otherwise, index is valid
	assert(index < bm->length, "Invalid access in bitmap.");
	// index/8 its the index of the byte that contains the bit
	// 7-(index%8) its the dislocation of the bit in the byte
	bit=bit & 0x01;
	bit=bit<<(7-(index%8));
	bm->contents[index/8]= bm->contents[index/8] | bit;
}

/**
 * Add a bit at the end of the bitmap.
 * @param bm The bitmap.
 * @param bit The new bit value.
 * @pre bitmapGetLength(bm) < bitmapGetMaxSize(bm)
 * @post (bitmapGetBit(bm,bitmapGetLength(bm) @ pre)==bit) 
 * and (bitmapGetLength(bm) == bitmapGetLength(bm) @ pre+1)
 */
void bitmapAppendLeastSignificantBit(bitmap* bm, unsigned char bit) {
	// verify if vm->length<bm->max_size, if otherwise, bitmap is full
	assert(bm->length < bm->max_size, "Maximum sized exceed in bitmap.");
	// as we are adding a bit, we should increase the size of the bitmap
	bm->length++;
	bitmapSetBit(bm, bm->length-1, bit);
}


/**
 * Free the bitmap
 * @pre The map exists.
 * @post The map was freed from memory
 */
void bitmapFree(bitmap bm) {
	free(bm.contents);
}
