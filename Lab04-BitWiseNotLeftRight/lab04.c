#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Converts a string of '0'/'1' characters into the unsigned integer value it represents.
 * Input: bit_string - a null terminated string containing only '0' and '1'
 *        length - the number of characters in bit_string
 * Output: the unsigned integer value represented by bit_string
 * Assumption: 1 <= length <= 64, and bit_string contains only '0'/'1'.
 */
unsigned long long bit_string_to_value(const char *bit_string, int length)
{
  unsigned long long value;
  int index;

  value = 0;
  for (index = 0; index < length; index = index + 1)
    {
      value = (value * 2) + (unsigned long long)(bit_string[index] - '0');
    }


  return value;
}

/*
 * Builds the bitmask that keeps only the lowest 'length' bits of a value.
 * Input: length = the number of bits the mask should cover
 * Output: an unsigned long long with the lowest 'length' bits set to 1
 * and all higher bits set to 0
 * Assumption: 1 <= length <= 64
 */
unsigned long long build_bit_mask(int length)
{
  unsigned long long mask;

  if (length == 64)
    {
      mask = ~0ULL;
    }
  else
    {
      mask = (1ULL << length) - 1ULL;
    }

  return mask;
}

/*
 * Converts a unsigned integer value into a string of '0'/'1' characters of a
 * fixed length, padding on the left with '0' characters as needed.
 * Input: value - the unsigned integer value to convert
 *        length - the desired length of the resulting bit string
 *	  result - a caller-provided buffer of at least length + 1 bytes
 * Ouput: none
 * Assumption: 1 <= length <= 64 and result has enough space
 */
void value_to_bit_string(unsigned long long value, int length, char *result)
{
  int index;
  int bit_position;
  unsigned long long current_bit;

  for (index = 0; index < length; index = index + 1)
    {
      bit_position = length - 1 - index;
      current_bit = (value >> bit_position) & 1ULL;
      result[index] = (char)('0' + current_bit);
    }
  result[length] = '\0';
}

/*
 * Prints the two-line result summary shared by all three operations.
 * Input: operand_bits - the bit string form of the shift/rotate amount to print
 *        symbol - the operator symbol to display
 *        original_bits - the original operand as a bit string
 *        result_bits - the resulting value as a bit string
 *        original_value - the original operand as an unsigned integer
 *        operand_value - the shift amount as a decimal integer
 *        result_value - the resulting value as an unsigned integer
 *        length - the bit string length (n) for this run
 *        is_not - nonzero if this is the -not operation, zero otherwise
 * Ouput: none
 */
void print_result(const char *original_bits, const char *symbol,
		  const char *operand_bits, const char *result_bits,
		  unsigned long long original_value, int operand_value,
		  unsigned long long result_value, int length, int is_not)
{
  if (is_not)
    {
      printf("%s%s evaluates to %s using bit strings of length %d\n",
	     symbol, original_bits, result_bits, length);
      printf("%s%llu evaluates to %llu using unsigned %d-bit integers\n",
	     symbol, original_value, result_value, length);
    }
  else
    {
      printf("%s %s %s evaluates to %s using bit strings of length %d\n",
	     original_bits, symbol, operand_bits, result_bits, length);
      printf("%llu %s %d evaluates to %llu using unsigned %d-bit integers\n",
	     original_value, symbol, operand_value, result_value, length);
    }
}

/*
 * Program entry point. Parses command line arguments for one of three forms, performs
 * bitwise operation, and prints the result in both bit string and base-10 form.
 * Input: argc - the number of command line arguments
 *        argv - array of command line argument strings
 * Ouput: returns 0 on success, 1 if the arguments are invalid
 */
int main(int argc, char*argv[])
{
  const char *bit_string;
  int length;
  unsigned long long original_value;
  unsigned long long mask;
  unsigned long long result_value;
  char result_bits[65];

  if (argc == 3 && strcmp(argv[1], "-not") == 0)
    {
      bit_string = argv[2];
      length = (int)strlen(bit_string);
      mask = build_bit_mask(length);

      original_value = bit_string_to_value(bit_string, length);
      result_value = (~original_value) & mask;

      value_to_bit_string(result_value, length, result_bits);
      print_result(bit_string, "~", "", result_bits,
		   original_value, 0, result_value, length, 1);
    }
  else if (argc == 4 &&
	   (strcmp(argv[2], "-leftshift") == 0 ||
	    strcmp(argv[2], "-rightshift") == 0))
    {
      int shift_amount;
      char shift_bits[65];
      const char *symbol;

      bit_string = argv[1];
      length = (int)strlen(bit_string);
      mask = build_bit_mask(length);
      shift_amount = atoi(argv[3]);

      original_value = bit_string_to_value(bit_string, length);

      if (strcmp(argv[2], "-leftshift") == 0)
	{
	  symbol = "<<";
	  result_value = (original_value << shift_amount) & mask;
	}
      else
	{
	  symbol = ">>";
	  result_value = (original_value >> shift_amount) & mask;
	}

      value_to_bit_string((unsigned long long)shift_amount, length, shift_bits);
      value_to_bit_string(result_value, length, result_bits);
      print_result(bit_string, symbol, shift_bits, result_bits,
		   original_value, shift_amount, result_value, length, 0);
    }
  else
    {
      fprintf(stderr, "Usage: %s -not <bitstring>\n", argv[0]);
      fprintf(stderr, "       %s <bitstring> -leftshift <amount>\n", argv[0]);
      fprintf(stderr, "       %s <bitstring> -rightshift <amount>\n", argv[0]);
      return 1;
    }

  return 0;
}
