#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/* Global variable: environ
 * Points to the enviroment of the current process. It is defined by the C library and is passed
 * as the third argument to both execve calls so that ps and countdown.out run with the same
 *enviroment
 */
extern char **environ;

/* Prints the parents PID, then calls fork a single time to create a child process. The child prints its own
 * PID and uses execve to run ps. The parent waits for the child to finish, then uses execve to run
 * ./countdown.out with the integer on the command line.
 * Input: argc - the number of command line arguments
 *        argv - the command line arguments; argv[1] is the starting number for the countdown.
 * Output: Prints the PID and status message, the output of ps, and the output of countdown.out to standard
 * output. Returns EXIT_FAILURE if fork, wait, or execve fails. If both execve calls succeed, main does not return.
 *
 * Assumptions: argv[1] is a valid integer greater than 0. ps is located at /bin/ps, and countdown.out has been compiled.
 */
int main(int argc, char *argv[]) {
  pid_t pid;
  int status;

  if (argc < 2) {
    printf("Usage: %s <integer greater than 0>\n", argv[0]);
    return EXIT_FAILURE;
  }

  printf("The parent's PID is %d.\n", getpid());
  printf("The parent is now forking.\n");
  fflush(stdout);

  pid = fork();

  if (pid < 0) {
    printf("The call to fork failed.\n");
    return EXIT_FAILURE;
  } else if (pid == 0) {
    char *psArgs[] = {"ps", NULL};
    printf("The child's PID is %d.\n", getpid());
    printf("The child is executing ps.\n");
    fflush(stdout);
    execve("/bin/ps", psArgs, environ);
    printf("The call to execve for ps failed.\n");
    exit(EXIT_FAILURE);
  } else {
    char *countdownArgs[] = {"./countdown.out", argv[1], NULL};
    if (wait(&status) == -1) {
      printf("The call to wait failed.\n");
      return EXIT_FAILURE;
    }
    printf("The parent waited patiently for its child to complete.\n");
    printf("The parent is executing ./countdown.out %s.\n", argv[1]);
    fflush(stdout);
    execve("./countdown.out", countdownArgs, environ);
    printf("The call to execve for countdown failed.\n");
    return EXIT_FAILURE;
  }
}
