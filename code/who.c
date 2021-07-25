#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

void show_time(time_t tv) {
  char cp[17];
  struct tm *tp = localtime(&tv);
  strftime(cp, 26, "%Y-%m-%d %H:%M", tp);
  printf("%s", cp);
}

void show_info(struct utmp *utbufp) {
  if (utbufp->ut_type != USER_PROCESS) return;
  printf("%-8.8s\t%-8.8s\t", utbufp->ut_user, utbufp->ut_line);
  show_time(utbufp->ut_tv.tv_sec);
  if (utbufp->ut_host[0] != '\0') {
    printf(" (%s)\n", utbufp->ut_host);
  }
}

int main() {
  struct utmp current_record;
  int utmpfd;
  int reclen = sizeof(current_record);
  if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) {
    perror(UTMP_FILE);
    exit(1);
  }
  while (read(utmpfd, &current_record, reclen) == reclen)
    show_info(&current_record);
  close(utmpfd);
  return 0;
}