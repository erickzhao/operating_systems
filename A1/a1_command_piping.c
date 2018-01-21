#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  int   fd[2];
  char  buf;
  char* ls[] = {"ls", NULL};

  pipe(fd);

  if (fork() == 0) {
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    execvp(ls[0], ls);
    close(fd[1]);
  } else {
    close(fd[1]);
    while (read(fd[0], &buf, 1) > 0) {
      printf("%c", buf);
    }
    close(fd[0]);
  }

  return 0;
}

