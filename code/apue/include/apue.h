#ifndef _APUE_H
#define _APUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAXLINE 4096
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void err_ret(const char *, ...);
void err_sys(const char *, ...) __attribute__((noreturn));
void err_quit(const char *, ...) __attribute__((noreturn));

#endif /* _APUE_H */
