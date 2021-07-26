#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 4096  // 缓存大小
#define COPYMODE 0644  // 文件权限

// 输出错误信息
void oops(char *s1, char *s2) {
  fprintf(stderr, "Error: %s ", s1);
  perror(s2);
  exit(1);
}

int main(int argc, char *argv[]) {
  int in_fd, out_fd, n_chars;
  char buf[BUFFSIZE];

  if (argc != 3) {
    fprintf(stderr, "usage: %s source dest\n", *argv);
    exit(1);
  }

  if ((in_fd = open(argv[1], O_RDONLY)) == -1) {  // 打开源文件
    oops("Cannot open", argv[1]);
  }
  if ((out_fd = creat(argv[2], COPYMODE)) == -1) {  // 创建目标文件
    oops("Cannot create", argv[2]);
  }

  while ((n_chars = read(in_fd, buf, BUFFSIZE)) > 0) {  // 从源文件中读取
    if (write(out_fd, buf, n_chars) != n_chars) {       // 写入目标文件
      oops("Write error to", argv[2]);
    }
  }
  if (n_chars == -1) {
    oops("Read errro from", argv[1]);
  }

  if (close(in_fd) == -1 || close(out_fd) == -1) {
    oops("Error closing files", "");
  }
}