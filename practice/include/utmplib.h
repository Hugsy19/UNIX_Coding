#ifndef _UTMPLIB_H_
#define _UTMPLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <utmp.h>

#define NRECS 16 // 缓存数量
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE]; // 缓存内容
static int num_recs, cur_rec;
static int fd_utmp = -1;

int utmp_open(char *);
struct utmp *utmp_next();
int utmp_reload();
void utmp_close();

#endif