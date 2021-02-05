实验总结

1. 配置环境
	切换git checkout
2. gdb不出现
	解决方案：https://zhuanlan.zhihu.com/p/331492444
	直接在新的窗口输入
	riscv64-unknown-elf-gdb kernel/kernel 
	得到gdb出现了
	再在窗口输入
	target remote localhost:25501端口号
	
	所以我们只要去用户目录下的.gdbinit文件里添加下面这行就好了（注意不是xv6目录下的），如果没有这个文件就自己创建一个。
 	文件配置
	add-auto-load-safe-path /home/fs/xv6-labs-2020/.gdbinit
	
3. gdb不打印调试信息
	Python Exception <type 'exceptions.NameError'> Installation error: gdb._execute_unwinders function is missing: 
	解决方案：iscv64-unknown-elf-gdb kernel/kernel --data-directory=pwd/gdb/data-directory
	添加--data-directory=/usr/local/Cellar/riscv-gnu-toolchain/master/share/gdb
	该方案不管用
	
	改用：http://kcmetercec.top/2018/01/29/linux_debug_gdb_problem/
	中的方法，在/usr/local/Cellar/riscv-gnu-toolchain/master/share/gdb中复制了/usr/local/Cellar/gdb/10.1/share/gdb的代码，
	然后出现不出调试信息中的代码，在/users/xxx/.gdbint中设置代码文件的位置
	add-auto-load-safe-path /Users/XXX/xv6-labs-2020/.gdbinit
  	
	重新启动就可以看到正常的信息了
	
4.具体实验参考别人的代码
4.1 sys trace
     本次实验主要理解，如何写kernel代码。
     a.最先要做的必然是添加$U/_trace到Makefile中的UPROGS变量里
     b.用户态的代码sys_trace直接调用了系统函数trace，所以需要我们写trace函数
     c.因为要编译时,识别user/trace.c，需要添加声明到user/user.h，(添加 int trace(int);)添加一个entry到user/usys.pl( print "#include \"kernel/syscall.h\"\n"; 添加 entry("trace");)
     和一个syscall number到kernel/syscall.h中，这就完成系统函数添加的过程；
     
     然后就到了实现kernel中trace的部分，在 kernel/sysproc.c中添加sys_trace() 函数，用于传递参数，
     该参数是之前没有的，需要在proc structure (see kernel/proc.h)中添加一下，int mask；
     然后修改kernel/syscall.c，在syscall中添加syscall的字符串数组，添加mask和syscall之间的代码。
     
     最后，不要忘记修改fork（kernel/proc.c）的代码，在子进程中也传递参数
     
4.2 sysinfo
	流程和4.1类似
     a.最先要做的必然是添加$U/_sysinfotest到Makefile中的UPROGS变量里
     b.用户态的代码sys_sysinfo直接调用了系统函数sysinfo，所以需要我们写sysinfo函数
     c.因为要编译时,识别user/sysinfotest.c，需要添加声明到user/user.h，(添加 int sysinfo(int);)添加一个entry到user/usys.pl( print "#include \"kernel/syscall.h\"\n"; 添加 entry("sysinfo");)和一个syscall number到kernel/syscall.h中，这就完成系统函数添加的过程；
     然后就到了实现kernel中sysinfo的部分，在 kernel/sysproc.c中添加sys_sysinfo() 函数，用于传递参数，判断参数用的是argaddr函数，读进来我们要保存的在用户态的数据sysinfo的指针地址。sysinfo needs to copy a struct sysinfo back to user space; see sys_fstat() (kernel/sysfile.c) and filestat() (kernel/file.c) for examples of how to do that using copyout().
     然后写两个新函数，一个用于计算内存（free memory, add a function to kernel/kalloc.c），一个用于计算系统进程个数（processes, add a function to kernel/proc.c）；
     最后利用copyout函数，将内核态中的信息复制到用户态。
     
   注意事项：
   记得free_mem和free_proc函数要加入到kernel/defs.h头文件里声明一下；
   还有#include "sysinfo.h"需要在kernel/sysproc.c和kernel/syscall.h中添加；
     

5.最后测试分数差一点，在文件夹下touch time.txt就通过了


代码参考
1.【MIT-6.S081-2020】Lab2 syscall - lemonman的文章 - 知乎
https://zhuanlan.zhihu.com/p/274369780
2.MIT 6.S081 2020 Lab2 system calls讲解 - 北海草鱼的文章 - 知乎
https://zhuanlan.zhihu.com/p/332243456

---
时间:2021年02月05日09:32:48
