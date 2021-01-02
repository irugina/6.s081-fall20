#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc == 0)  exit(1);

  int clicks = atoi(argv[1]);
  sleep(clicks);
  exit(0);
}
