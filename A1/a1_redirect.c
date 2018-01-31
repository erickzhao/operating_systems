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
  // APPEND - appends text to file
  // CREAT  - Creates file if not found in system
  int fd = open(FILE_NAME, O_WRONLY | O_APPEND | O_CREAT, 0666);

  printf("First: Print to stdout\n");
  dup2(fd, STDOUT_FILENO); // overwrite stdout with file
  printf("Second: Print to redirect_out.txt\n");
  dup2(stdout_cpy, STDOUT_FILENO); // put stdout back in place
  printf("Third: Print to stdout\n");
  
  return 0;
}