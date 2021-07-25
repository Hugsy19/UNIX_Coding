# 实现more

## 分析

- Linux中可用`more`分页显式文件的内容，期间可进行的基本操作有：
  - 空白键（Space）：下一页
  - 回车（Enter）：下一行
  - q：结束显式
- `more`一般有三种用法：
  - `more filename`：显式文件内容
  - `command | more`：将`command`的输出内容分页显示
  - `more < filename`：从标准输入获得需分页显式的内容
- 由此，`more`的基本工作流程如下：
  1. 读取输入文件，输出一页内容；
  2. 打印提示信息：`[more?]`；
  3. 等待用户输入：
    * `Enter`：输出下一行，回到2；
    * `Space`：回到1；
    * `q`：退出。

## 实现

```c
#include <stdio.h>
#include <stdlib.h>

#define PAGELINE 24 // 每页的行书
#define LINELEN 512 // 每行的字符数

// 打印提示信息，处理用户输入
int see_more(FILE *cmd) {
  int c;
  printf("\033[7m more? \033[m"); // 反白打印"more?"
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
  if ((fp_tty = fopen("/dev/tty", "r")) == NULL) exit(1); // 打开键盘和显示器的设备描述文件
  while (fgets(line, LINELEN, fp)) { // 从输入文件中读取一行，缓存到字符数组
    if (num_of_lines == PAGELINE) { // 已经输出了一页数据
      reply = see_more(fp_tty); // 从键盘输读取用户输入
      if (reply == 0) break;
      num_of_lines -= reply; 
    }
    if (fputs(line, stdout) == EOF) exit(1); // 输出缓存在字符数组中数据
    ++num_of_lines;
  }
}

int main(int argc, char *argv[]) {
  FILE *fp;
  if (argc == 1) {
    do_more(stdin); // 没有额外参数，以标准输入作为输入
  } else {
    while (--argc) {
      if ((fp = fopen(*++argv, "r")) != NULL) { // 以只读的方式打开文件
          do_more(fp);
          fclose(fp); // 关闭文件
      } else {
          exit(1);
      }
    }
  }
  return 0;
}
```
- 相比原指令，以上程序还有几个问题待解决：
  - 用户输入后必须使用回车；
  - "more?"提示会随屏幕滚动；
  - 没有实现显式读取进度。