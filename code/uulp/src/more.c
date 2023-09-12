#include <stdio.h>
#include <stdlib.h>

#define PAGELINE 24  // 每页的行书
#define LINELEN 512  // 每行的字符数

// 打印提示信息，处理用户输入
int see_more(FILE *cmd) {
  int c;
  printf("\033[7m more? \033[m");  // 反白打印"more?"
  while ((c = getc(cmd)) != EOF) {
    if (c == 'q') return 0;
    if (c == ' ') return PAGELINE;
    if (c == '\n') return 1;
  }
}

// 读取文件内容并输出到显示器
void do_more(FILE *fp) {
  char line[LINELEN];
  int num_of_lines = 0;
  int reply;
  FILE *fp_tty;
  if ((fp_tty = fopen("/dev/tty", "r")) == NULL)
    exit(1);  // 打开键盘和显示器的设备描述文件
  while (fgets(line, LINELEN, fp)) {  // 从输入文件中读取一行，缓存到字符数组
    if (num_of_lines == PAGELINE) {  // 已经输出了一页数据
      reply = see_more(fp_tty);      // 从键盘读取用户输入
      if (reply == 0) break;
      num_of_lines -= reply;
    }
    if (fputs(line, stdout) == EOF) exit(1);  // 输出缓存在字符数组中数据
    ++num_of_lines;
  }
}

int main(int argc, char *argv[]) {
  FILE *fp;
  if (argc == 1) {
    do_more(stdin);  // 没有额外参数，以标准输入作为输入
  } else {
    while (--argc) {
      if ((fp = fopen(*++argv, "r")) != NULL) {  // 以只读的方式打开文件
        do_more(fp);
        fclose(fp);  // 关闭文件
      } else {
        exit(1);
      }
    }
  }
  return 0;
}