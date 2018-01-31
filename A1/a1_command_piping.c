#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  int   fd[2];
  char  buf;
  char* ls[] = {"ls", NULL};

  // store pointers to pipe inside fd array.
  // fd[0] - read
  // fd[1] - write
  pipe(fd);

  pid_t pid = fork();

  if (pid == 0) {
    // child
    close(fd[0]); // close unused read end
    dup2(fd[1], STDOUT_FILENO); // set write end as STDOUT
    execvp(ls[0], ls);
    close(fd[1]); // close write end after ls executes
  } else {
    // parent
    close(fd[1]); // close unused write end

    // read single characters until there aren't any more
    while (read(fd[0], &buf, 1) > 0) {
      printf("%c", buf);
    }
    close(fd[0]); // close read end after input is read
  }

  return 0;
}

