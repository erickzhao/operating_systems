#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int STDOUT_FH = 1;
char * file_name = "redirect_out.txt";

int main() {
  int stdout_cpy = dup(STDOUT_FH);
  int fd = open(file_name, O_WRONLY | O_APPEND | O_CREAT, 0666);

  printf("First: Print to stdout\n");
  dup2(fd, STDOUT_FH);
  printf("Second: Print to redirect_out.txt\n");
  dup2(stdout_cpy, STDOUT_FH);
  printf("Third: Print to stdout\n");
  return 0;
}