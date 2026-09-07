#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Pads a bit string with leading zeros so it reaches targetLength.
 * Input: the original bit string and the desired total length
 * Output: a newly allocated, null-terminated string of length targetLength
 * Assumes targetLength is >= the length of original
 */
char *padBitString(char *original, int targetLength);

/*
 * Converts a bit string of given length into its unsigned decimal value.
 * Input: a bit string and its length.
 * Output: the unsigned long long value it represents.
 * Assumes the string contains only '0' and '1' characters.
 */
unsigned long long bitStringToDecimal(char *bits, int length);

/*
 * Converts an unsigned decimal value into a bit string of a given length.
 * Input: the value to convert and the desired bit-string length.
 * Output: a newly allocated, null-terminated bit string.
 * Assumes value fits within 'length' bits.
 */
char *decimalToBitString(unsigned long long value, int length);

/*
 * Determines the display symbol associated with a command line flag.
 * Input: the flag string (and, or, exclusive or).
 * Output: the corresponding character symbol.
 * Assumes flag is one of the three valid values.
 */
char getOperatorSymbol(char *flag);

/*
 * Program entry point. Reads two bit strings and an operator flag from the
 * command line, pads the shorter bit string, applies the requested bitwise
 * operator, and prints the result in both binary and decimal form.
 * Input: command line arguments (bit string, flag, bit string).
 * Output: return 0 on success.
 * Assumes exactly 3 command line arguments and are valid.
 */
int main(int argc, char *argv[]) {
  char *firstBitString = argv[1];
  char *operatorFlag = argv[2];
  char *secondBitString = argv[3];

  int firstLength = strlen(firstBitString);
  int secondLength = strlen(secondBitString);
  int maxLength = firstLength;
  if (secondLength > firstLength) {
    maxLength = secondLength;
  }

  char *paddedFirst = padBitString(firstBitString, maxLength);
  char *paddedSecond = padBitString(secondBitString, maxLength);

  unsigned long long firstValue = bitStringToDecimal(paddedFirst, maxLength);
  unsigned long long secondValue = bitStringToDecimal (paddedSecond, maxLength);

  unsigned long long resultValue = 0;
  if (strcmp(operatorFlag, "-and") == 0) {
    resultValue = firstValue & secondValue;
  } else if (strcmp(operatorFlag, "-or") == 0) {
    resultValue = firstValue | secondValue;
  } else if (strcmp(operatorFlag, "-xor") == 0) {
    resultValue = firstValue ^ secondValue;
  }

  char *resultBitString = decimalToBitString(resultValue, maxLength);
  char operatorSymbol = getOperatorSymbol(operatorFlag);

  printf("%s %c %s evaluates to %s using bit strings of length %d\n",
	 paddedFirst, operatorSymbol, paddedSecond, resultBitString, maxLength);
  printf("%llu %c %llu evaluates to %llu using unsigned %d-bit integers\n",
	 firstValue, operatorSymbol, secondValue, resultValue, maxLength);

  free(paddedFirst);
  free(paddedSecond);
  free(resultBitString);

  return 0;
}

char *padBitString(char *original, int targetLength) {
  int originalLength = strlen(original);
  int paddingNeeded = targetLength - originalLength;

  char *paddedString = malloc(targetLength + 1);

  for (int i = 0; i < paddingNeeded; i++) {
    paddedString[i] = '0';
  }
  
  for (int i = 0; i < originalLength; i++) {
    paddedString[paddingNeeded + i] = original[i];
  }
  paddedString[targetLength] = '\0';

  return paddedString;
}

unsigned long long bitStringToDecimal(char *bits, int length) {
  unsigned long long value = 0;

  for (int i = 0; i < length; i++){
    value = value * 2;
    if (bits[i] == '1') {
      value = value + 1;
    }
  }

  return value;
}

char *decimalToBitString(unsigned long long value, int length) {
  char *bitString = malloc(length + 1);
  bitString[length] = '\0';

  for (int position = length - 1; position >= 0; position--) {
    if (value % 2 == 0) {
      bitString[position] = '0';
    } else {
      bitString[position] = '1';
    }
    value = value / 2;
  }

  return bitString;
}

char getOperatorSymbol(char *flag) {
  char symbol = '&';

  if (strcmp(flag, "-and") == 0) {
    symbol = '&';
  } else if (strcmp(flag, "-or") == 0) {
    symbol = '|';
  } else if (strcmp(flag, "-xor") == 0) {
    symbol = '^';
  }

  return symbol;
}
		       
