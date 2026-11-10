#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "proj4.h"

void initializeGrid(grid * g, char * fileName){
  FILE * inFile = fopen(fileName, "r");
  unsigned int row, col;
  int ch = fgetc(inFile);
  g->n = 0;
  while(ch != '\n' && ch != EOF){
    g->n++;
    ch = fgetc(inFile);
  }
  rewind(inFile);
  g->p = malloc(g->n * sizeof(unsigned char *));
  for(row = 0; row < g->n; row ++){
    g->p[row] = malloc(g->n * sizeof(unsigned char));
    for(col = 0; col < g->n; col++){
      g->p[row][col] = (unsigned char) fgetc(inFile);
    }
    fgetc(inFile);
  }
  fclose(inFile);
}

void processDiagonal(grid * input, grid * output, unsigned long s,
                     int startRow, int startCol, int colStep){
  int n = (int) input->n;
  int left = 0;
  int right = 0;
  int markedUpTo = 0;
  int k;
  unsigned long windowSum = 0;
  while(startRow + right < n && startCol + right * colStep >= 0 &&
        startCol + right * colStep < n){
    windowSum += input->p[startRow + right][startCol + right * colStep] - '0';
    while(windowSum > s){
      windowSum -= input->p[startRow + left][startCol + left * colStep] - '0';
      left++;
    }
    if(windowSum == s){
      k = (left > markedUpTo) ? left : markedUpTo;
      while(k <= right){
        output->p[startRow + k][startCol + k * colStep] =
          input->p[startRow + k][startCol + k * colStep];
        k++;
      }
      markedUpTo = right + 1;
    }
    right++;
  }
}

void * diagonalSumsWorker(void * args){
  threadArgs * work = (threadArgs *) args;
  int n = (int) work->input->n;
  int diagonalsPerDirection = 2 * n - 1;
  int diagonal, offset;
  for(diagonal = work->threadIndex; diagonal < 2 * diagonalsPerDirection;
      diagonal += work->numThreads){
    if(diagonal < diagonalsPerDirection){
      offset = diagonal - (n-1);
      processDiagonal(work->input, work->output, work->s,
                      (offset < 0) ? -offset : 0, (offset > 0) ? offset: 0, 1);
    }
    else {
      offset = diagonal - diagonalsPerDirection;
      processDiagonal(work->input, work->output, work->s,
                      (offset > n - 1) ? offset - (n - 1) : 0,
                      (offset > n - 1) ? n - 1 : offset, -1);
    }
  }
  return NULL;
}

void diagonalSums(grid * input, unsigned long s, grid * output, int t){
  unsigned int row, col;
  int k;
  threadArgs args[3];
  pthread_t threads[2];

  output->n = input->n;
  output->p = malloc(output->n * sizeof(unsigned char *));
  for(row = 0; row < output->n; row++){
    output->p[row] = malloc(output->n * sizeof(unsigned char));
    for(col = 0; col < output->n; col++){
      output->p[row][col] = '0';
    }
  }

  for(k = 0; k < t; k++){
    args[k].input = input;
    args[k].output = output;
    args[k].s = s;
    args[k].threadIndex = k;
    args[k].numThreads = t;
  }

  for(k = 0; k < t - 1; k++){
    pthread_create(&threads[k], NULL, diagonalSumsWorker, &args[k]);
  }
  diagonalSumsWorker(&args[t - 1]);
  for(k = 0; k < t - 1; k++){
    pthread_join(threads[k], NULL);
  }
}

void writeGrid(grid * g, char * fileName){
  unsigned int row;
  int outFile = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  for(row = 0; row < g->n; row++){
    write(outFile, g->p[row], g->n);
    write(outFile, "\n", 1);
  }
  close(outFile);
}

void freeGrid(grid * g){
  unsigned int row;
  for(row = 0; row < g->n; row++){
    free(g->p[row]);
  }
  free(g->p);
}
