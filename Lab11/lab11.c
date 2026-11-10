#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 * Runs the command to the left of "-pipe" in a child process with its standard output sent into a pipe. The parent
 * waits for the child to finish, reads its standard input from the pipe, and runs the command to the right of "-pipe".
 *
 * Input: argc - the number of command line arguments
 *        argv - the command line arguments, in the form command [args] -pipe command [args]
 *
 * Ouput: Does not return if both execvp calls succeed. Prints an error message to standard output and returns 1 if a call fails.
 *
 * Assumptions: Input is valid and contains exactly one "-pipe" flag, with at least one command on each side of it.
 */
int main(int argc, char *argv[])
{
  int pipeIndex = 1;
  int pipeFds[2];
  pid_t childPid;

  while (strcmp(argv[pipeIndex], "-pipe") != 0) {
    pipeIndex++;
  }
  argv[pipeIndex] = NULL;

  if (pipe(pipeFds) == -1) {
    printf("Error: pipe failed.\n");
    return 1;
  }

  childPid = fork();
  if (childPid == -1) {
    printf("Error: fork failed.\n");
    return 1;
  }

  if (childPid == 0) {
    if (dup2(pipeFds[1], STDOUT_FILENO) == -1) {
      printf("Error: dup2 failed.\n");
      return 1;
    }
    close(pipeFds[0]);
    close(pipeFds[1]);
    execvp(argv[1], argv + 1);
    printf("Error: execvp failed.\n");
    return 1;
  }

  close(pipeFds[1]);
  if (wait(NULL) == -1) {
    printf("Error: wait failed.\n");
    return 1;
  }
  if (dup2(pipeFds[0], STDIN_FILENO) == -1) {
    printf("Error: dup2 failed.\n");
    return 1;
  }
  close(pipeFds[0]);
  execvp(argv[pipeIndex + 1], argv + pipeIndex + 1);
  printf("Error: execvp failed.\n");
  return 1;
}
