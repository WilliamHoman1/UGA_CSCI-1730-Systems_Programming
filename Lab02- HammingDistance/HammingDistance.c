/* lab02.c                                                                                                                                                                                                        
 * This program takes two non-negative integers as command line                                                                                                                                                   
 * arguments, converts each to its binary representation padding                                                                                                                                                  
 * with leading zeros so both strings are equal length, and computes                                                                                                                                              
 * the Hamming distance between the two bit strings.                                                                                                                                                              
 */


#include <stdio.h>
#include <stdlib.h>

/*                                                                                                                                                                                                                
 * countBits: Determines how many binary digits are needed to represent                                                                                                                                           
 * a given non-negative long value.                                                                                                                                                                               
 *                                                                                                                                                                                                                
 * Input: a non-negative long integer                                                                                                                                                                             
 * Output: the number of bits needed to represent it in binary                                                                                                                                                    
 * Assumption: number is non-negative                                                                                                                                                                             
 */
int countBits(long number)
{
  int bitCount = 0;

  if (number == 0)
    {
      bitCount =1;
    }

  while (number > 0)
    {
      bitCount = bitCount +1;
      number = number /2;
    }
  return bitCount;
}

/*                                                                                                                                                                                                                
 * numberToString: Converts a non-negative long into its binary representation,                                                                                                                                   
 * padded with leading zeros to match the given target length                                                                                                                                                     
 *                                                                                                                                                                                                                
 * Input: a non-negative long integer, and the target string length                                                                                                                                               
 * Output: none                                                                                                                                                                                                   
 * Assumption: bitString has room for targetLength characters plus a null                                                                                                                                         
 * terminator; targetLength is large enough to hold number's actual bit                                                                                                                                           
 * representation.                                                                                                                                                                                                
 */
void numberToString(long number, int targetLength, char *bitString)
{
  int position;

  bitString[targetLength] ='\0';

  for (position = targetLength - 1; position >= 0; position = position -1)
    {
      bitString[position] = (number % 2) + '0';
      number = number / 2;
    }
}

/*                                                                                                                                                                                                                
 * hammingDistance: Computes the Hamming distance between two equal-length                                                                                                                                        
 * bit strings.                                                                                                                                                                                                   
 *                                                                                   *                                                                                                                                                                                                                
 * Input: two null-terminated bit strings of the same length                                                                                                                                                      
 * Output: the number of positions where the two strings differ                                                                                                                                                   
 * Assumption: firstBitString and secondBitString are the same length                                                                                                                                             
 */
int hammingDistance(char *firstBitString, char *secondBitString, int length)
{
  int distance = 0;
  int position;

  for (position = 0; position < length; position = position + 1)
    {
      if (firstBitString[position] != secondBitString[position])
        {
          distance = distance + 1;
        }
    }

  return distance;

}

/*                                                                                                                                                                                                                
 * main: Entry point of the program. Reads two non-negative integers                                                                                                                                              
 * from the command line, converts each to a binary bit string,                                                                                                                                                   
 * padding with leading zeros so both are equal length, and prints                                                                                                                                                
 * both bit strings along with the Hamming distance between them.                                                                                                                                                 
 *                                                                                                                                                                                                                
 * Input: argc - the number of command line arguments                                                                                                                                                             
 * argv - the command line argument strings; argv[1] and                                                                                                                                                          
 * argv[2] must be non-negative integers convertible to long                                                                                                                                                      
 * via atol.                                                                                                                                                                                                      
 * Output: returns 0 on successful completion                      
 * Assumption: exactly two valid non-negative integer arguments are                                                                                                                                               
 * provided                                                                                                                                                                                                       
 */
int main(int argc, char *argv[])
{
  long firstNumber;
  long secondNumber;
  int firstBitLength;
  int secondBitLength;
  int maxBitLength;

  firstNumber = atol(argv[1]);
  secondNumber = atol(argv[2]);

  firstBitLength = countBits(firstNumber);
  secondBitLength = countBits(secondNumber);

  if (firstBitLength > secondBitLength)
    {
      maxBitLength = firstBitLength;
    }
  else
    {
      maxBitLength = secondBitLength;
    }

  char firstBitString[maxBitLength + 1];
  char secondBitString[maxBitLength + 1];

  numberToString(firstNumber, maxBitLength, firstBitString);
  numberToString(secondNumber, maxBitLength, secondBitString);

  printf("%s is the bit string for %ld\n", firstBitString, firstNumber);
  printf("%s is the bit string for %ld\n", secondBitString, secondNumber);
  printf("%d is the Hamming distance between the bit strings\n",
         hammingDistance(firstBitString, secondBitString, maxBitLength));

  return 0;
}
