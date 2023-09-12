#include "utmplib.h"

// 打开utmp文件
int utmp_open(char *filename) {
  fd_utmp = open(filename, O_RDONLY);
  cur_rec = num_recs = 0;
  return fd_utmp;
}

// 读入一批数据并放入缓存
int utmp_reload() {
  int amt_read;
  amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
  num_recs = amt_read / UTSIZE;  // 已读入的数据个数
  cur_rec = 0;
  return num_recs;
}

// 从缓存中读取一个utmp数据
struct utmp *utmp_next() {
  struct utmp *recp;
  if (fd_utmp == -1) return NULLUT;
  if (cur_rec == num_recs && utmp_reload() == 0) return NULLUT;
  recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];  // 缓存中读取一个
  ++cur_rec;                                         // 计数
  return recp;
}

// 关闭utmp文件
void utmp_close() {
  if (fd_utmp != -1) close(fd_utmp);
}