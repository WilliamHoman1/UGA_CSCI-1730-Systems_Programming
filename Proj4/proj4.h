#ifndef PROJ4_H
#define PROJ4_H

/*
 * The struct grid_t contains a pointer p to a 2D array of 
 * unsigned chars with n rows and n columns stored on
 * the heap of this process. Once this is initialized properly,
 * p[i][j] should be a valid unsigned char for all i = 0..(n-1)
 * and j = 0..(n-1).
 * Do not alter this typedef or struct in any way.
 */
typedef struct grid_t {
  unsigned int n;
  unsigned char ** p;
} grid;


/*
 * Initialize g based on fileName, where fileName
 * is a name of file in the present working directory
 * that contains a valid n-by-n grid of digits, where each
 * digit in the grid is between 1 and 9 (inclusive).
 * Do not alter this function prototype in any way.
 */
void initializeGrid(grid * g, char * fileName);


/*
 * This function will compute all diagonal sums in input that equal s using
 * t threads, where 1 <= t <= 3, and store all of the resulting
 * diagonal sums in output. Each thread should do
 * roughly (doesn't have to be exactly) (100 / t) percent of the 
 * computations involved in calculating the diagonal sums. 
 * This function should call (or call another one of your functions that calls)
 * pthread_create and pthread_join when 2 <= t <= 3 to create additional POSIX
 * thread(s) to compute all diagonal sums. 
 * Do not alter this function prototype in any way.
 */
void diagonalSums(grid * input, unsigned long s, grid * output, int t);


/*
 * Write the contents of g to fileName in the present
 * working directory. If fileName exists in the present working directory, 
 * then this function should overwrite the contents in fileName.
 * If fileName does not exist in the present working directory,
 * then this function should create a new file named fileName
 * and assign read and write permissions to the owner. 
 * Do not alter this function prototype in any way.
 */
void writeGrid(grid * g, char * fileName);

/*
 * Free up all dynamically allocated memory used by g.
 * This function should be called when the program is finished using g.
 * Do not alter this function prototype in any way.
 */
void freeGrid(grid * g);

/*
 * You may add any additional function prototypes and any additional
 * things you need to complete this project below this comment. 
 * Anything you add to this file should be commented. 
 */

/*
 * The struct threadArgs_t holds everything one thread needs to compute
 * its share of the diagonal sums.
 *
 * input: the grid being searched
 * output: the grid where digits that are part of a diagonal sum equal to
 *         s are written
 * s: the target diagonal sum
 * threadIndex: which thread this is (0 to numThreads - 1)
 * numThreads: the total number of threads computing diagonal sums
 */
typedef struct threadArgs_t {
  grid * input;
  grid * output;
  unsigned long s;
  int threadIndex;
  int numThreads;
} threadArgs;

/*
 * Thread start routine for computing diagonal sums.
 * Input: args, a pointer to a threadArgs struct.
 * Output: always returns NULL. The grid has 2n - 1 down-right diagonals and 2n - 1
 * down-left diagonals, numbered 0 to 4n - 3. This thread processes every diagonal whose
 * number is threadIndex plus a multiple of numThreads, so the work is split roughly evenly
 * between threads.
 * Assumes output has already been allocated with the same n as input and filled with 0.
 */
void * diagonalSumsWorker(void * args);

/*
 * Finds every contigous run of cells of one full diagonal whose digits sum to s,
 * and copies the digits of those runs from input into output. The diagonal starts
 * at (startRow, startCol) on the edge of the grid and moves down one row and colStep
 * columns per step until it leavees the grid. Uses a sliding window, so it runs in time
 * linear in the diagonal's length.
 *
 * Input: input and output grids, target sum s, the diagonal's starting cell
 *        and colStep (1 for down-right, -1 for down-left).
 * Output: none
 * Assumes: (startRow, startCol) is inside the grid, every digit is 1-9,
 *          and output has the same n as input.
 */
void processDiagonal(grid * input, grid * output, unsigned long s,
                     int startRow, int startCol, int colStep);




#endif

