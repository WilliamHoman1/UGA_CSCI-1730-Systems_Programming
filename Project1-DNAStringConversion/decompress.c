#include <stdlib.h>
#include "decompress.h"

/*
 * Converts a single 2-bit code back into its corresponding DNA base character.
 * Input: a 2-bit value (0-3)
 * Output: the corresponding base character of a DNA string
 * Assumptions: code is 0,1,2,3
 */
char code_to_base(unsigned char code){
  char base = 'A';
  if (code == 0) {
    base = 'A';
  } else if (code == 1) {
    base = 'T';
  } else if (code == 2) {
    base = 'C';
  } else if (code == 3) {
    base = 'G';
  }
  return base;
}

/*
 * Decompress an array of bytes back into the original DNA string,
 * extracting four 2-bit codes per byte and converting each back into its
 * corresponding base character. Stops after producing original_length
 * characters, ignoring any padding bits.
 * Input: bytes - an array of unsigned char holding the compressed data
 *        num_bytes - the number of bytes in the bytes array
 *        original_length - the number of characters in the original, uncompressed DNA string
 * Output: an allocated, null-terminated string containing the reconstructed DNA string
 * Assumptions: bytes are produced by compress() and correctly encodes a DNA string
 */
char *decompress(unsigned char *bytes, int num_bytes, int original_length) {
  char *dna = malloc((original_length + 1) * sizeof(char));

  int byte_index;
  int char_index = 0;
  for (byte_index = 0; byte_index < num_bytes; byte_index++) {
    unsigned char current_byte = bytes[byte_index];
    int slot;
    for (slot = 3; slot >= 0; slot--) {
      if (char_index < original_length) {
	unsigned char code = (current_byte >> (slot * 2)) & 3;
	dna[char_index] = code_to_base(code);
	char_index++;
      }
    }
  }

  dna[original_length] = '\0';
  return dna;
}
