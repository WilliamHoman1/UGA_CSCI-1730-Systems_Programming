#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
/*
 * Holds the number of the kill signal (8,10, or 12) that was handled.
 * Stays 0 until one of those signals arrives. Declared volatile sig_atomic_t
 * because it is written inside a signal handler.
 */
volatile sig_atomic_t caughtSignal = 0;

/*
 * Signal handler for kill -8, kill -10, and kill -12.
 * Input: signalNumber - the number of the signal that was delivered.
 * Output: none - stores signalNumber in the global caughtSignal.
 * Assumptions: only registered for signals 8, 10, and 12.
 */
void handleSignal(int signalNumber) {
  caughtSignal = signalNumber;
}

/*
 * Sets up the signal handlers, prints the pid, and sums doubles read from standard
 * input one line at a time. Ends when a kill signal is handled, or sleeps for 60 seconds
 * once the input ends.
 * Input: none (reads from standard input)
 * Output: prints the sum and how the program ended; returns 0.
 * Assumptions: input may be typed by a user or redirected from a file.
 */
int main(void) {
  struct sigaction action = {0};
  char *line = NULL;
  size_t lineCapacity = 0;
  double sum = 0.0;
  double value = 0.0;
  char extraCharacter;
  int processId = (int) getpid();

  action.sa_handler = handleSignal;
  sigemptyset(&action.sa_mask);
  sigaction(8, &action, NULL);
  sigaction(10, &action, NULL);
  sigaction(12, &action, NULL);

  printf("Program started with pid = %d.\n", processId);
  printf("Enter a list of doubles to sum,\n");
  printf("and to end the program,\n");
  printf("run one of the following Unix commands:\n");
  printf("kill -8 %d\n", processId);
  printf("kill -10 %d\n", processId);
  printf("kill -12 %d\n", processId);

  while (caughtSignal == 0 && getline(&line, &lineCapacity, stdin) != -1) {
    if (sscanf(line,"%lf %c", &value, &extraCharacter) == 1) {
      sum += value;
    } else {
      printf("Error: please input a numeric value.\n");
    }
  }
  free(line);

  if (caughtSignal == 0) {
    sleep(60);
  }

  printf("The sum is %f.\n", sum);
  if (caughtSignal != 0) {
    printf("Program ended by handling the signal from kill -%d %d.\n",
	   (int) caughtSignal, processId);
  } else {
    printf("Program ended after sleeping for 60 seconds.\n");
  }

  return 0;
}
  
