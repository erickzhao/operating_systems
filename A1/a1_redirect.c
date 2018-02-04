/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
*/ 

// Please enter your name and McGill ID below
// Name: Erick Zhao
// McGill ID: 260687719

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char * FILE_NAME = "redirect_out.txt";

int main() {

  // keep copy of stdout
  int stdout_cpy = dup(STDOUT_FILENO);
  // WRONLY - allows read and write of file
  // CREAT  - Creates file if not found in system
  int fd = open(FILE_NAME, O_WRONLY | O_CREAT, 0666);

  printf("First: Print to stdout\n");
  dup2(fd, STDOUT_FILENO); // overwrite stdout with file
  printf("Second: Print to redirect_out.txt\n");
  dup2(stdout_cpy, STDOUT_FILENO); // put stdout back in place
  printf("Third: Print to stdout\n");

  return 0;
}