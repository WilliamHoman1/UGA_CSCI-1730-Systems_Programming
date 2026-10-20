#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * step 1
 * Compares the two files one byte at a time and writes every byte of file1 that differs from the byte at the same
 * position in file2 to differencesFoundInFile1.txt.
 * Input: fileName1, fileName2 - paths to the two input files.
 * Output: 0 on success, 1 on read error, 2 on write error.
 * Assumptions: uses buffers of size 2
 */
int step1(char *fileName1, char *fileName2);

/*
 * step 2
 * Reads both files into exactly-sized dynamic arrays, copies every byte of file2 that differs from file1
 * into a third dynamic array, and writes that array to differencesFoundInFile2.txt.
 * Input: fileName1, fileName2 - paths to the two input files.
 * Output: 0 on success, 1 on a read error, 2 on a write error.
 * Assumptions: all memory is freed before returning.
 */
int step2(char *fileName1, char *fileName2);

/*
 * readFile
 * Allocates an array the exact size of a file and reads the whole file into it.
 * Input: fileName - path of the file to read
 *        size - set to the file's size of the bytes.
 * Output: the allocated array, or NULL if the file is empty or on error.
 * Assumptions: the caller frees the returned array.
 */
char *readFile(char *fileName, off_t *size);

/*
 * main
 * Checks the arguments, runs and times step1 and step2, and prints both times or an error message.
 * Input: argc - argument count; argv - argv[1] and argv[2] are the files.
 * Output: 0 on success, 1 on any error
 * Assumptions: n/a
 */
int main(int argc, char *argv[])
{
  struct timeval start;
  struct timeval end;
  double time1 = 0;
  double time2 = 0;
  int result = 0;

  if (argc != 3) {
    printf("Usage: proj3.out <file1> <file2>\n");
    return 1;
  }

  gettimeofday(&start, NULL);
  result = step1(argv[1],argv[2]);
  gettimeofday(&end, NULL);
  time1 = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

  if (result == 0) {
    gettimeofday(&start, NULL);
    result = step2(argv[1], argv[2]);
    gettimeofday(&end, NULL);
    time2 = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
  }

  if (result == 1) {
    printf("There was an error reading a file.\n");
  } else if (result == 2) {
    printf("There was an error writing to a file.\n");
  } else {
    printf("Step 1 took %f milliseconds\n", time1);
    printf("Step 2 took %f milliseconds\n", time2);
  }
  return result != 0;
}

int step1(char *fileName1, char *fileName2)
{
  int result = 0;
  int fd1 = open(fileName1, O_RDONLY);
  int fd2 = open(fileName2, O_RDONLY);
  int outFd = -1;
  char buffer1[2] = {0,0};
  char buffer2[2] = {0,0};
  ssize_t read1 = 0;
  ssize_t read2 = 0;

  if (fd1 == -1 || fd2 == -1) {
    result = 1;
  } else {
    outFd = open("differencesFoundInFile1.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (outFd == -1) {
      result = 2;
    }
  }

  if (result == 0) {
    read1 = read(fd1, buffer1, 1);
  }
  while (result == 0 && read1 == 1) {
    read2 = read(fd2, buffer2, 1);
    if (read2 == -1) {
      result = 1;
    } else if ((read2 == 0 || buffer1[0] != buffer2[0]) && write(outFd, buffer1, 1) != 1) {
      result = 2;
    } else {
      read1 = read(fd1, buffer1, 1);
    }
  }
  if (result == 0 && read1 == -1) {
    result = 1;
  }

  if (fd1 != -1) {
    close(fd1);
  }
  if (fd2 != -1) {
    close(fd2);
  }
  if (outFd != -1) {
    close(outFd);
  }
  return result;
}

int step2(char *fileName1, char *fileName2)
{
  int result = 0;
  off_t size1 = 0;
  off_t size2 = 0;
  off_t count = 0;
  off_t i = 0;
  int outFd = -1;
  char *file1 = readFile(fileName1, &size1);
  char *file2 = readFile(fileName2, &size2);
  char *differences = NULL;

  if (size1 == -1 || size2 == -1) {
    result = 1;
  } else {
    differences = malloc(size2 > 0 ? size2: 1);
    outFd = open("differencesFoundInFile2.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (differences == NULL || outFd == -1) {
      result = 2;
    }
  }

  if (result == 0) {
    for (i = 0; i <size2; i++) {
      if (i >= size1 || file2[i] != file1[i]) {
	differences[count] = file2[i];
	count++;
      }
    }
    if (write(outFd, differences, count) != count) {
      result = 2;
    }
  }

  if (outFd != -1) {
    close(outFd);
  }
  free(file1);
  free(file2);
  free(differences);
  return result;
}

char *readFile(char *fileName, off_t *size)
{
  struct stat info;
  char *contents = NULL;
  int fd = open(fileName, O_RDONLY);

  *size = -1;
  if (fd != -1 && stat(fileName, &info) == 0) {
    if (info.st_size > 0) {
      contents = malloc(info.st_size);
    }
    if (info.st_size == 0 || (contents != NULL && read(fd, contents, info.st_size) == info.st_size)) {
      *size = info.st_size;
    }
  }
  if (*size == -1) {
    free(contents);
    contents = NULL;
  }
  if (fd != -1) {
    close(fd);
  }
  return contents;
}

