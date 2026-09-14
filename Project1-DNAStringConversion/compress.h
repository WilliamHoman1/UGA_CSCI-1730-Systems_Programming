#ifndef COMPRESS_H
#define COMPRESS_H

/*
 * Compresses a DNA string into an array of bytes, packing four 2-bit-encoded bases
 * into each byte. If the string length is not a multiple of 4, the final byte is
 * padded with zero bits in the low-order positions.
 * Input: dna - a null-terminated string containing 'A','T','C','G', with length > 0.
 * Output: a allocated array of unsigned char holding the compressed representation
 * Assumptions: dna is valid, non-empty DNA string
 */
unsigned char *compress(const char *dna, int *num_bytes);

/*
 * Converts a single DNA base character into its 2-bit encoding.
 * Input: one character, a base letter of a DNA string
 * Ouput: the 2-bit code for that base, returned as an unsigned char
 * Assumptions: base is one of the four valid DNA characters
 */
unsigned char base_to_code(char base);

#endif
