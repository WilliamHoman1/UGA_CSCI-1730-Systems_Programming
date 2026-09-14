#ifndef DECOMPRESS_H
#define DECOMPRESS_H

/*
 * Decompresses an array of bytes back into the original DNA string, extracting four 2-bit
 * codes per byte and converting each back into its corresponding base character. Stops
 * after producing original_length characters, ignoring any padding bits in the final byte.
 *
 * Input: bytes - an array of unsigned char holding the compressed data
 *        num_bytes - the number of bytes in the bytes array
 *        original_length - the number of bytes in the bytes array
 *
 * Output: an allocated, null-terminated string containing the reconstructed DNA string
 *
 * Assumptions: bytes was produced by compress() and correctly encodes a DNA string of
 * length original_length
 */
char *decompress(unsigned char *bytes, int num_bytes, int original_length);

/*
 * Converts a single 2-bit code back into its corresponding DNA base character.
 *
 * Input: a 2-bit value in the low-order bits of an unsigned char
 *
 * Ouput: the corresponding base character of a DNA string
 *
 * Assumptions: the code is one of 0,1,2, or 3
 */
char code_to_base(unsigned char code);

#endif
 
