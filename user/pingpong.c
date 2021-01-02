#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
  int fd[2];
  pipe(fd);
  int pid = fork();
  if (pid > 0){
    char *ping = "a";
    write(fd[1], ping, 1);
    pid = wait( (int*) 0);
    char *pong = "a";
    read(fd[0], pong, 1);
    pid = getpid();
    printf("%d: received pong\n", pid);
    exit(0);
  }
  else if (pid == 0){
    char *ping = "a";
    read(fd[0], ping, 1);
    pid = getpid();
    printf("%d: received ping\n", pid);
    write(fd[1], ping, 4);
    exit(0);
  }
  exit(0);
}
