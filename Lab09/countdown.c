#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>  //For usleep function

/*
 * This program will read an integer > 0 inputted
 * as the second command line argument
 * and print a countdown to standard output.
 */
int main(int argc, char * argv[]){
  int i = 0;
  int retVal = 0;
  printf("Starting the countdown!\n");
  if(argc >= 2){
    retVal = sscanf(argv[1], "%d", &i);
  }
  if(retVal == 1 && i > 0){
    while(i > 0){
      printf("%d\n", i);  
      fflush(stdout); 
      usleep(250 * 1000);  //Sleep for 250 ms
      i--;
    }
    printf("Countdown finished.\n");
  }
  else{
    printf("Error:  second command line argument must be an integer greater than 0.\n");
  } 
  return 0;
}


