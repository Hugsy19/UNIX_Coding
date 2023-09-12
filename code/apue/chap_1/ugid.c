// apue 1-9

#include <unistd.h>

#include "apue.h"

int main(int argc, char *argv[]) {
  printf("uid = %d, gid = %d\n", getuid(), getgid());
  exit(0);
}