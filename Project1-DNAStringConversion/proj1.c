#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"
#include "decompress.h"

/*
 * Parses command line arguments, then calls compress() or decompress ()
 * based on the flag given, and prints the result.
 * Input: argc - number of command line arguments
 *        argv - argv[1] is "-c" or "-d", remaining args depend on the flag
 * Output: 0 on completion
 * Assumptions: inputs are valid
 */
int main(int argc, char *argv[]) {
  if (strcmp(argv[1], "-c") == 0) {
    int num_bytes;
    unsigned char *compressed = compress(argv[2], &num_bytes);
    printf ("%d", (int)strlen(argv[2]));
    int i;
    for (i = 0; i < num_bytes; i++) {
      printf(" %d", compressed[i]);
    }
    printf("\n");
    free(compressed);
  } else if (strcmp(argv[1], "-d") == 0) {
    int original_length = atoi(argv[2]);
    int num_bytes = argc - 3;
    unsigned char *bytes = malloc(num_bytes * sizeof(unsigned char));
    int i;
    for (i = 0; i < num_bytes; i++) {
      bytes[i] = (unsigned char) atoi(argv[i + 3]);
    }
    char *dna = decompress(bytes, num_bytes, original_length);
    printf("%s\n", dna);
    free(bytes);
    free(dna);
  }
  return 0;
}
