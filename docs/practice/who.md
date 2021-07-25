### 分析

- Linux中`who`命令可用来查看当前系统上在线的所有用户。
  - 从左到右依次显式用户名、终端名、登陆时间、登录地址，如：
    ```
    hugsy    tty2         2021-07-25 14:12 (tty2)
    root     pts/2        2021-07-25 13:58 (192.168.255.1)
    root     pts/1        2021-07-25 14:14 (192.168.255.1)
    ```
- 通过`man who`查看man手册中的相关信息，可知`who`命令一般是通过读取`/var/run/utmp`中的数据，来获得当前的用户相关信息。
- 再通过`man -k umtp`搜索utmp的相关信息，可找到与之相关的一条`utmp (5)`，通过`man 5 utmp`读取文档可知：
  - utmp文件保存的是结构数组，数组元素都是`utmp`类型的数据；
  - `utmp`类型定义在`<utmp.h>`中，具体与`libc`的实现相关。
- Linux中各种系统相关的头文件一般都放在`/usr/include`目录下，glibc-2.31中定义的`utmp.h`被拆分成了两部分：
  - `/usr/include/utmp.h`：定义了相关函数
  - `/usr/include/bits/utmp.h`：定义了`utmp`类型
- glibc-2.31中`utmp`类型的具体定义如下：
  ```c
  struct utmp
  {
    short int ut_type;        /* Type of login.  */
    pid_t ut_pid;         /* Process ID of login process.  */
    char ut_line[UT_LINESIZE];    /* Devicename. 终端名 */
    char ut_id[4];    /* Inittab ID.  */
    char ut_user[UT_NAMESIZE];    /* Username. 用户名 */
    char ut_host[UT_HOSTSIZE];    /* Hostname for remote login. 登录地址 */
    struct exit_status ut_exit;   /* Exit status of a process marked as DEAD_PROCESS.  */
    long int ut_session;      /* Session ID, used for windowing.  */
    struct timeval ut_tv;     /* Time entry was made. 登录时间 */
    int32_t ut_addr_v6[4];    /* Internet address of remote host.  */
    char __glibc_reserved[20];        /* Reserved for future use.  */
  };
  ```

### 实现

- 基本的实现过程如下：
```c
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>

// 显式时间
void show_time(time_t tv) {
  char cp[17];
  struct tm *tp = localtime(&tv); // 将秒数转换为本地时间
  strftime(cp, 26, "%Y-%m-%d %H:%M", tp); // 控制显式格式
  printf("%s", cp);
}

// 显式各种信息
void show_info(struct utmp *utbufp) {
  if (utbufp->ut_type != USER_PROCESS) return; // 过滤非用户进程
  printf("%-8.8s\t%-8.8s\t", utbufp->ut_user, utbufp->ut_line); // 打印用户名、终端名
  show_time(utbufp->ut_tv.tv_sec); // 打印时间
  if (utbufp->ut_host[0] != '\0') {
    printf(" (%s)\n", utbufp->ut_host); // 打印登录地址
  }
}

int main() {
  struct utmp current_record;
  int utmpfd;
  int reclen = sizeof(current_record);
  if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1) { // 打开utmp文件 
    perror(UTMP_FILE);
    exit(1);
  }
  while (read(utmpfd, &current_record, reclen) == reclen) // 每次读取size(utmp)大小的内容
    show_info(&current_record);
  close(utmpfd); // 关闭utmp文件
  return 0;
}
```
- 利用缓冲机制，可以提到文件的读取效率：
```c
```