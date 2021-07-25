#include <time.h>

#include "utmplib.h"

// 显式时间
void show_time(time_t tv) {
  char cp[17];
  struct tm *tp = localtime(&tv);          // 将秒数转换为本地时间
  strftime(cp, 26, "%Y-%m-%d %H:%M", tp);  // 控制显式格式
  printf("%s", cp);
}

// 显式各种信息
void show_info(struct utmp *utbufp) {
  if (utbufp->ut_type != USER_PROCESS) return;  // 过滤非用户进程
  printf("%-8.8s\t%-8.8s\t", utbufp->ut_user,
         utbufp->ut_line);          // 打印用户名、终端名
  show_time(utbufp->ut_tv.tv_sec);  // 打印时间
  if (utbufp->ut_host[0] != '\0') {
    printf(" (%s)\n", utbufp->ut_host);  // 打印登录地址
  }
}

int main() {
  struct utmp *utbufp;
  if (utmp_open(UTMP_FILE) == -1) {  // 打开utmp文件
    perror(UTMP_FILE);
    exit(1);
  }
  while ((utbufp = utmp_next()) != NULLUT)  // 从缓存中读取一条数据
    show_info(utbufp);
  utmp_close();  // 关闭utmp文件
  return 0;
}