// apue 1-8

#include <errno.h>

#include "apue.h"

int main(int argc, char *argv[]) {
  fprintf(stderr, "EACCES: %s\n", strerror(EACCES));
  errno = ENOENT;
  perror(argv[0]);
  exit(0);
}