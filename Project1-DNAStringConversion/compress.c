#include <stdlib.h>
#include <string.h>
#include "compress.h"

/*
 * Coverts a single DNA base character into its 2-bit encoding
 * Input: one character, a base letter of a DNA string
 * Output: the 2-bit code for that base, returned as an unsigned char
 * Assumptions: base is one of the four valid DNA characters
 */
unsigned char base_to_code(char base){
  unsigned char code = 0;
  if (base == 'A') {
    code = 0;
  } else if (base == 'T') {
    code = 1;
  } else if (base == 'C') {
    code = 2;
  } else if (base == 'G') {
    code = 3;
  }
  return code;
}

/*
 * Compresses a DNA string into an array of bytes, packing four 2-bit encoded
 * bases into each byte. If the string length is not a multiple of 4, the final
 * byte is padded with zero bits in the low-order positions.
 * Input: a null terminated string containing DNA base > 0.
 * Ouput: an allocated array of unsigned char holding the compressed representation.
 * Assumptions: dna is valid
 */
unsigned char *compress(const char *dna, int *num_bytes) {
  int length = strlen(dna);
  int total_bytes = (length + 3) / 4;
  unsigned char *compressed = malloc(total_bytes * sizeof(unsigned char));

  int byte_index;
  for (byte_index = 0; byte_index < total_bytes; byte_index++) {
    unsigned char current_byte = 0;
    int slot;
    for (slot = 0; slot < 4; slot++) {
      int char_index = byte_index * 4 + slot;
      unsigned char code = 0;
      if (char_index < length) {
	code = base_to_code(dna[char_index]);
      }
      current_byte = (current_byte << 2) | code;
    }
    compressed[byte_index] = current_byte;
  }

  *num_bytes = total_bytes;
  return compressed;
}
 
