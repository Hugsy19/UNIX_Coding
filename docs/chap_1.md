# 基础知识

## 登录

- 用户的登录信息一般都保存在`/etc/passwd`中，各用户的登录项一般由7个以冒号分隔的字段组成。
  - 例如`root:x:0:0:root:/root:/bin/bash`；
  - 它们依次表示：登录名、密码、用户id、组id、注释、home目录、shell程序。
- 用户登录后即可向shell程序键入命令，UNIX中常见的shell有：
  - Bourne shell：即`/bin/sh`，由Steve Bourne在贝尔实验室开发；
  - C shell：即`/bin/csh`，由Bill Joy在伯克利开发，支持一些sh没有的特色功能；
  - Korn shell：即`/bin/ksh`，由贝尔实验室的David Korn开发，兼容sh并支持csh的特殊功能；
  - Bourne-again shell：即目前Linux中最常用的`/bin/bash`，设计遵循POSIX，保持与sh的兼容性的同时支持以上两者的特色功能。

## 文件与目录

- UNIX文件系统中，所有文件与目录都以根目录`/`为起点。
- 目录是包含目录项的文件，大多数UNIX实现中，文件属性都不会放在目录项中。
  - 因为当某个文件具有多个硬链接时，那样难以在多个副本间保持属性的同步。
- 创建新目录时都会自动新建两个文件`.`和`..`，分别指向当前目录和其父目录，且根目录中两者都指向当前目录。
- `<dirent.h>`中定义了`opendir`、`readdir`等可用来打开、读取目录信息的函数原型。
- 各进程都有一个工作目录，所有想对路径都是从工具目录开始解释。

## 输入和输出

- UNIX内核都用**文件描述符**（file description）来标识已打开的文件，它是一个小的非负整数。
- 运行新程序时，shell都会默认开启三个文件描述符：
  - 标准输入（standard input）
  - 标注输出（standard output）
  - 标准错误（standard error）
- I/O有带缓冲和不带缓冲之分，`open`、`close`、`write`、`lseek`及`close`都提供不带缓冲的I/O。
  - 它们的函数原型和几个标准I/O文件描述符一起，都定义在`<unistd.h>`。
- C标准库中定义的`printf`、`get`、`put`等标准I/O函数，则为不带缓冲的I/O提供了带缓冲的接口。

## 程序和进程



## 错误处理

## 用户

## 信号

## 时间

## 系统调用与库函数