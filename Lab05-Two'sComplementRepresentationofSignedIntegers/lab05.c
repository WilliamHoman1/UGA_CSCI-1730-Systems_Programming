/*
 * CSCI 1730 - Lab 05
 * Two's Complement Representation of Signed Integers
 *
 * This program converts signed decimal integers and their
 * n-bit two's complement binary representation, based on command
 * line arguments.
 *
 * Usage:
 *      ./lab05.out -decimal <x> -bits <n>
 *      ./lab05.out -bits <n> -decimal <x>
 *                 Converts the signed decimal integer x into its n-bit
 *                 two's complement binary representation.
 *
 *      ./lab05.out -binary <bitString>
 *                 Converts the given two's complement bit string into its
 *                 signed decimal (base-10) value. The number of bits n is taken
 *                 to be the length of bitString.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Builds the n-bit two's complement binary representation of the signed decimal value x
 * and stores it (as a null-terminated string of '0'/'1' characters, most significant bit first)
 * into outBits.
 *
 * Input: x - the signed decimal integer to convert
 *        n - the number of bits to use (1 <= n <= 64)
 *        outBits - a caller-allocated buffer of at least n + 1 chars
 * Output: none (result written into outBits)
 * Assumes: -2^(n-1) <= x <= 2^(n-1) - 1
 */
void decimalToTwosComplementBits(long long x, int n, char *outBits)
{
  unsigned long long bitMask;
  unsigned long long representation;
  int bitPosition;

  
  if (n == 64)
    {
      bitMask = ~0ULL;
    }
  else
    {
      bitMask = (1ULL << n) - 1ULL;
    }


  representation = ((unsigned long long) x) & bitMask;

  for (bitPosition = n -1; bitPosition >= 0; bitPosition--)
    {
      unsigned long long bitValue;

      bitValue = (representation >> bitPosition) & 1ULL;
      outBits[n - 1 - bitPosition] = (char) ('0' + bitValue);
    }

  outBits[n] = '\0';
}

/*
 * Converts a n-bit two's complement bit string into its signed
 * decimal (base-10) value.
 * Input: bits - a null terminated string of '0'/'1' characters,
 *               most significant bit first
 *        n - the number of bits (the length of bits)
 * Output: the signed decimal value represented by bits
 * Assumes: 1 <= n <= 64, and bits contains only '0' and '1' characters
 */
long long twosComplementBitsToDecimal(const char *bits, int n)
{
  unsigned long long unsignedValue;
  long long signedValue;
  int index;
  bool isNegative;

  unsignedValue = 0ULL;
  for (index = 0; index < n; index++)
    {
      unsigned long long currentBit;

      currentBit = (unsigned long long) (bits[index] - '0');
      unsignedValue = (unsignedValue << 1) | currentBit;
    }

  isNegative = (bits[0] == '1');

  if (n == 64)
    {
      signedValue = (long long) unsignedValue;
    }
  else if (isNegative)
    {
      signedValue = (long long) unsignedValue - (1LL << n);
    }
  else
    {
      signedValue = (long long) unsignedValue;
    }

  return signedValue;
}

/*
 * Program entry point. Scans the command line arguments for the -decimal, -bits, and -binary flags
 * (which may appear in any order), performs the requested conversion, and prints the result.
 *
 * Input: argc - the number of command line arguments
 *        argv - the command line argument strings
 * Ouput: returns 0 on successful completion
 * Assumes: the command line arguments follow the correct format
 */
int main(int argc, char *argv[])
{
  const char *decimalString;
  const char *bitsString;
  const char *binaryString;
  int argIndex;

  decimalString = NULL;
  bitsString = NULL;
  binaryString = NULL;

  for (argIndex = 1; argIndex < argc; argIndex++)
    {
      if (strcmp(argv[argIndex], "-decimal") == 0 && argIndex + 1 < argc)
	{
	  decimalString = argv[argIndex + 1];
	}
      else if (strcmp(argv[argIndex], "-bits") == 0 && argIndex + 1 < argc)
	{
	  bitsString = argv[argIndex + 1];
	}
      else if (strcmp(argv[argIndex], "-binary") == 0 && argIndex + 1 < argc)
	{
	  binaryString = argv[argIndex + 1];
	}
    }

  if (binaryString != NULL)
    {
      int numBits;
      long long decimalValue;

      numBits = (int) strlen(binaryString);
      decimalValue = twosComplementBitsToDecimal(binaryString, numBits);

      printf("%s in binary is equal to %lld in decimal using %d-bit two's complement representation\n",
      binaryString, decimalValue, numBits);
      
    }
  else if (decimalString != NULL && bitsString != NULL)
    {
      long long decimalValue;
      int numBits;
      char binaryBits[65];

      decimalValue = strtoll(decimalString, NULL, 10);
      numBits = atoi(bitsString);

      decimalToTwosComplementBits(decimalValue, numBits, binaryBits);

      printf("%lld in decimal is equal to %s in binary using %d-bit two's complement representation\n",
	     decimalValue, binaryBits, numBits);
    }

   return 0;
}
