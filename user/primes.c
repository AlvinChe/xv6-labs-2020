#include "kernel/types.h"
#include "user/user.h"

#define R 0
#define W 1

void panic(char *s)
{
  fprintf(STDDER_FILENO, "%s\n", s);
  exit(1);
}

int fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}


// Redirect standard input or standard output to pipe
void redirect(int k, int p[])
{
    close(k);
    dup(p[k]);
    close(p[R]);
    close(p[W]);
}

void filter(int p)
{
    int n;

    while (read(R, &n, sizeof(n)))
    {
        if (n % p != 0)
        {
            write(W, &n, sizeof(n));
        }
    }
    close(W);
    // very tricky, or the main primes would return after the second process exit.
    wait(0);
    exit(0);
}

// The last process, waitting for a number
void primes()
{
    int pd[2];
    int p;
    // 如果一直有数据传进来
    if (read(R, &p, sizeof(p)))
    {
    	// 第一个传进来的数据一定是质数
        printf("prime %d\n", p);
        pipe(pd);
        // 对数据进行过滤和传输
        if (fork() == 0)    // child
        {
        	// 子进程重定向读数据的来源
            redirect(R, pd);
            // 递归
            primes();
        }
        else    // parent
        {
        	// 父进程重定向写数据的去处
            redirect(W, pd);
            // 过滤数据
            filter(p);
        }
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int pd[2];

    pipe(pd);

    int pid = fork1();
    // 子进程
    if (pid == 0)    //child
    {
    	// 重定向子进程的读数据来源
        redirect(R, pd);
        // 等待管道传来的数据
        primes();
    }
    else    // parent
    {
    	// 重定向 父进程的 写数据
        redirect(W, pd);
        // 向子进程发送2-35的数值
        for (int i = 2; i < 36; i++)
        {
            write(W, &i, sizeof(i));
        }
        // close the write side of pipe, cause read returns 0
        close(W);
        // the main primes process should wait until all the other primes process exited
        wait(0);
        exit(0);
    }
    return 0;
}