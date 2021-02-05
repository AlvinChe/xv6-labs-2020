####实验心得
1.sleep
 最简单，只要调用已有参数
 难点：走通整个过程；在makefile中写入sleep函数，这个地方如果空格没有写对会无法编译；运行代码之前需要make clean，不然总是出现奇怪的bug

2.pingpog
 需要仔细阅读xv6的第一章，理解I/O的抽象；
 理解为什么要关掉写端和读端；
 
3.primes
 最难，最后完成。
 primes的原理不难理解，但是非常不好写，递归的版本总是因为无法写对wait()和exit(0)而导致调试不通；参考几个代码写了好几个版本，有的成功，有的失败。
 递归的版本简洁漂亮，非递归的版本，利用了一个数组，也非常有创意。

4.find
 直接复制粘贴sh.c
 需要读明白sh.c，然后进行改进；需要熟悉c语言对字符串的处理

5.xargs
 处理时分成两部分，先处理xargs后面带的参数，即argv[]中的参数；然后处理从管道中传过来的参数，管道里的是数据流，所以对需要对字符串进行分割，按照“ ”和“/n”来划分；最后利用新的进程，exec执行所有命令

时间：2021年02月02日16:44:48
---

课程主页：https://pdos.csail.mit.edu/6.828/2020/schedule.html
实验一util主页：https://pdos.csail.mit.edu/6.828/2020/labs/util.html

代码参考：
1.https://zhuanlan.zhihu.com/p/272199762
2.https://github.com/zhayujie/xv6-riscv-fall19/
3.https://github.com/gaofanfei/xv6-riscv-fall20/

参考资料
1. xargs：http://www.ruanyifeng.com/blog/2019/08/xargs-tutorial.html
2. learn sys ：https://learn-sys.github.io/
