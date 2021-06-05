/** Defines an ADT representig a bitmap.
 * @file bitmap.h
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br)
 */

#ifndef BITMAP_H_
#define BITMAP_H_

/**
 * Structure to represent a bitmap.
 */
typedef struct {
	unsigned int max_size;		///< max size in bits
	unsigned int length; 		///< current size in bits
	unsigned char* contents; 	///< bitmap content
} bitmap;

unsigned char* bitmapGetContents(bitmap bm);
unsigned int bitmapGetMaxSize(bitmap bm);
unsigned int bitmapGetLength(bitmap bm);
bitmap bitmapInit(unsigned int max_size);
unsigned char bitmapGetBit(bitmap bm, unsigned int index);
void bitmapSetBit(bitmap* bm, unsigned int index, unsigned char bit);
void bitmapAppendLeastSignificantBit(bitmap* bm, unsigned char bit);

void bitmapFree(bitmap bm);

#endif /*BITMAP_H_*/
