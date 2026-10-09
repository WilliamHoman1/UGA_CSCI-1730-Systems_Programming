#include <stdio.h>
#include <stdlib.h>

// p is a pointer that is used to test the sum function.
int * p;

/*
 * Return the sum of the n values in x starting at index 0.
 * If x is NULL, then return -1.
 * If n is 0 and x is not NULL, then return 0.
 */
int sum(int * x, int n);

/*
 * main reads integers from the command line, stores them in a heap
 * array, prints the first value and their sum, then tests sum with
 * a NULL pointer.
 * 
 */
int main(int argc, char * argv[]){
  // allocate argc ints (argc - 1 numbers plus one zeroed slot so *x
  // is 0 when no numbers are given). calloc sets every slot to 0
  int * x = calloc(argc, sizeof(int));
  // convert each arugment to an int
  for(int i = 0; i < argc - 1; i++){
    x[i] = atoi(argv[i + 1]);
  }

  // print the first value, the sum of all values, and the NULL test
  // Examples:
  // ./lab07.out -> *x is 0, y is 0, z is -1
  // ./lab07.out 8 1 2 -> *x is 8, y is 11, z is -1
  // ./lab07.out 21 -> *x is 21, y is 21, z is -1
  
  printf("*x is %d\n", *x);
  int y = sum(x, argc - 1);
  printf("y is %d\n", y);
  int z = sum(p, argc - 1);
  printf("z is %d\n", z);
  
  // release the heap array so there are no memory leaks
  free(x);

  return 0;
}

int sum(int * x, int n){
  if(x == NULL){
    return -1;
  }
  int total = 0;
  for (int i = 0; i < n; i++){
	total = total + x[i];
      }
  return total;
}

