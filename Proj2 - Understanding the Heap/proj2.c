#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Number of grades added to the capacity each time the heap block grows. */
#define GROWTH_SIZE 5

/* Running totals of heap usage, reported at the end of the program */
static int totalAllocs = 0;
static int totalFrees = 0;
static long totalBytes = 0;

/*
 * Reads grades from standard input until a negative value, stores them on the heap,
 * prints the average and how each grade compares to it, then prints the total heap usage.
 * Input: None (grades are read from standard input)
 * Output: Returns 0 on successful completion.
 * Assumptions: All inputs are valid doubles and the list ends with a negative value.
 */
int main(void);

/*
 * Allocates room for capacity doubles on the heap and prints the allocation.
 * Input: capacity - the number of doubles to allocate (assumed > 0).
 * Ouput: pointer to the new block. Updates the heap usage counters.
 * Assumptions: Capacity is greater than 0 and malloc succeeds.
 */
double *allocateGrades (int capacity);

/*
 * Frees a block of grades and prints how many bytes were freed.
 * Input: grades - block to free, capacity - number of doubles in the block.
 * Output: none. Updates the heap counters.
 * Assumptions: Grades was allocated by allocateGrades and has not already been freed.
 */
void freeGrades(double *grades, int capacity);

/*
 * Copies the grades from one heap block to another using pointer arithmetic.
 * Input: source - the block to copy from. destination - the block to copy to
 *        count - the number of grades to copy
 * Output: None
 * Assumptions: Both blocks hold at least count doubles and do not overlap.
 */
void copyGrades(const double *source, double *destination, int count);

/*
 * Grows a full block of grades by allocating a block with room for GROWTH_SIZE
 * more grades, copying the old grades over, and freeing the old block.
 * Input: grades - the full block. capacity - pointer to the block's current capacity.
 * Output: Returns a pointer to the new, larger block and increases *capacity by GROWTH_SIZE.
 * Assumptions: The block is completley full.
 */
double *growGrades(double *grades, int *capacity);

/*
 * Prints whether each grade is >= or < the average, numbered from 1.
 * Input: grades - the block of grades. count - the number of grades
 *        average - the average to compare against
 * Output: None
 * Assumptions: grades hold at least count doubles.
 */
void printComparisons(const double *grades, int count, double average);

/*
 * Computes the average of a list of grades.
 * Input: grades - the block of grades. count - the number of grades
 * Output: Returns the average, or 0.0 if count is 0.
 * Assumptions: grades hold at least count doubles (grades may be NULL if count is 0).
 */
double computeAverage(const double *grades, int count);

int main(void)
{
  double *grades = NULL;
  int count = 0;
  int capacity = 0;
  double input = 0.0;
  double average = 0.0;

  printf("Enter a list of grades below where each grade is separated by a newline character.\n");
  printf("After the last grade is entered, enter a negative value to end the list. \n");

  while (scanf("%lf", &input) == 1 && input >= 0) {
    if (grades == NULL) {
      capacity = GROWTH_SIZE;
      grades = allocateGrades(capacity);
    }
    *(grades + count) = input;
    printf("Stored %f in the heap at %p.\n", input, (void *) (grades + count));
    count++;
    if (count == capacity) {
      grades = growGrades(grades, &capacity);
    }
  }

  average = computeAverage(grades, count);
  printf("The average of %d grades is %f.\n", count, average);
  printComparisons(grades, count, average);

  if (grades != NULL) {
    freeGrades(grades, capacity);
  }
  printf("total heap usage: %d allocs, %d frees, %ld bytes allocated\n",
	 totalAllocs, totalFrees, totalBytes);
  return 0;
}

double *allocateGrades(int capacity)
{
  int bytes = capacity * (int) sizeof(double);
  double *block = malloc(bytes);
  totalAllocs++;
  totalBytes += bytes;
  printf("Allocated %d bytes to the heap at %p.\n", bytes, (void *) block);
  return block;
}

void freeGrades(double *grades, int capacity)
{
  int bytes = capacity * (int) sizeof(double);
  printf("Freed %d bytes from the heap at %p.\n", bytes, (void *) grades);
  free(grades);
  totalFrees++;
}

void copyGrades(const double *source, double *destination, int count)
{
  int index;
  for (index = 0; index < count; index++) {
    *(destination + index) = *(source + index);
  }
}

double *growGrades(double *grades, int *capacity)
{
  int oldCapacity = *capacity;
  double *newGrades;

  printf("Stored %d grades (%d bytes) to the heap at %p.\n",
	 oldCapacity, oldCapacity * (int) sizeof(double), (void *) grades);
  printf("Heap at %p is full.\n", (void *) grades);

  *capacity = oldCapacity + GROWTH_SIZE;
  newGrades = allocateGrades(*capacity);
  copyGrades(grades, newGrades, oldCapacity);
  printf("Copied %d grades from %p to %p.\n",
	 oldCapacity, (void *) grades, (void *) newGrades);
  freeGrades(grades, oldCapacity);
  return newGrades;
}

double computeAverage(const double *grades, int count)
{
  double sum = 0.0;
  int index;
  for (index = 0; index < count; index++) {
    sum += *(grades + index);
  }
  return (count > 0) ? sum / count : 0.0;
}

void printComparisons(const double *grades, int count, double average)
{
  int index;
  for (index = 0; index < count; index++) {
    if (*(grades + index) >= average) {
      printf("%d. The grade of %f is >= the average.\n", index + 1, *(grades + index));
    } else {
      printf("%d. The grade of %f is < the average.\n", index + 1, *(grades + index));
    }
  }
}
