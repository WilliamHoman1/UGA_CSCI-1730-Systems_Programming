#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * FloatBits shares one 32 bit block of memory between a float and an int,
 * so the int memeber holds the exact IEEE 754 bit pattern of the float.
 */
union FloatBits {
  float floatValue;
  int intValue;
};

/*
 * DoubleBits shares one 64 bit block of memory between a double and a long,
 * so the long memeber holds the exact IEEE 754 bit pattern of the double.
 */
union DoubleBits {
  double doubleValue;
  long longValue;
};

/*
 * Prints bits of value from position height down to position lowBit,
 * most significant first. Bit 0 is the least significant bit.
 * Input: value to print, highBit, lowBit.
 * Ouput: none (prints to stdout, no newline).
 * Assumes 0 <= lowBit <= highBit <= 63.
 */
void printBits(long value, int highBit, int lowBit);

/*
 * Prints the full 32 bit IEEE 754 breakdown of a float: the whole bit pattern,
 * then the sign bit, exponent, and fraction on seperate lines.
 * Input: number, the float to encode.
 * Output: none (prints to stdout).
 * Assumptions: float and int are both 32 bits, and number has an exact binary
 * representation.
 */
void printFloat(float number);

/*
 * Prints the full 64 bit IEEE 754 breakdown of a double: the whole bit pattern,
 * then the sign bit, exponent, and fraction on seperate lines.
 * Input: number, the double to encode.
 * Output: none (prints to stdout).
 * Assumptions: double and long are both 64 bits, and number has an exact binary
 * representation.
 */
void printDouble(double number);

/*
 * Reads a flag and a value from the command line and prints the IEEE 754
 * encoding of the value as a float (-f) or a double (-d).
 * Input: argc, the arugment count; argv, the arguments
 * (argv[1] is -f or -d, argv[2] is the number).
 * Output: returns 0 on exit, prints the encoding to stdout.
 * Assumptions: exactly one valid flag and one valid number are given.
 */
int main(int argc, char *argv[]) {
  if (argc == 3 && strcmp(argv[1], "-f") == 0) {
    printFloat(strtof(argv[2], NULL));
  } else if (argc == 3 && strcmp(argv[1], "-d") == 0) {
    printDouble(strtod(argv[2], NULL));
  }
  return 0;
}

void printBits(long value, int highBit, int lowBit) {
  int bitIndex;
  for (bitIndex = highBit; bitIndex >= lowBit; bitIndex--) {
    printf("%ld", (value >> bitIndex) & 1L);
  }
}

void printFloat(float number) {
  union FloatBits converter;
  converter.floatValue = number;

  printf("%f encoded in binary using a 32 bit IEEE 754 encoding is below.\n", number);
  printBits(converter.intValue, 31, 0);
  printf("\nsign bit: ");
  printBits(converter.intValue, 31, 31);
  printf("\nexponent (8 bit): ");
  printBits(converter.intValue, 30, 23);
  printf("\nfraction (23 bit): ");
  printBits(converter.intValue, 22, 0);
  printf("\n");
}

void printDouble(double number) {
  union DoubleBits converter;
  converter.doubleValue = number;

  printf("%f encoded in binary using a 64 bit IEEE 754 encoding is below.\n", number);
  printBits(converter.longValue, 63, 0);
  printf("\nsign bit: ");
  printBits(converter.longValue, 63, 63);
  printf("\nexponent (11 bit): ");
  printBits(converter.longValue, 62, 52);
  printf("\nfraction (52 bit): ");
  printBits(converter.longValue, 51, 0);
  printf("\n");
}
