#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDDER_FILENO 2

#define READEND 0
#define WRITEEND 1


// user/sh.c
int fork1(void);  // Fork but panics on failure.
void panic(char*);

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

void closepipe(int *pd)
{
	close(pd[READEND]);
	close(pd[WRITEEND]);
} 

void primes()
{
	int prime,num,len;
	// printf("Into primes\n");
	// 第一步从数据中读出prime
	if ( (len=read(STDIN_FILENO,&prime,sizeof(int)) )<=0 || prime<=0)
	{
		close(STDOUT_FILENO);
		exit(1);
	}else{
		// panic("number error");
		// fprintf(STDDER_FILENO,"prime %d\n",prime);
		printf("prime %d\n",prime);
	}
	
	// 第二步递归分流
	int fd[2];
	// 新建一个管道，用于向下分数据；
	pipe(fd);
	int pid = fork1();
	if (pid >0)
	{
		// 父进程
		// 重定向数据写的方向
		close(STDOUT_FILENO);
		dup(fd[WRITEEND]);
		closepipe(fd);
		// 重定向写端之后，向pipe里面传不能取余的数据
		while(1)
		{
			// fprintf(STDDER_FILENO,"get from pipe %d\n", num);
			if((len = read(STDIN_FILENO,&num,sizeof(int)) )<=0 || num <=0){
				close(STDOUT_FILENO);
				exit(1);
			}else{
				if (num%prime!=0)
				{
					write(STDOUT_FILENO,&num,sizeof(int));
				}
			}
		}
		close(STDOUT_FILENO);
		
		wait(0);
		exit(0);
	}else{
		// 子进程
		// 重定向数据读的方向
		close(STDIN_FILENO);
		dup(fd[READEND]);
		closepipe(fd);
		// 递归处理质数
		primes();
	}
	exit(0);
}

int main(int argc, char *argv[])
{
	int fd[2];
	int pid;
	int i;

	// 创建管道用于传输数据
	pipe(fd);
	pid = fork1();
	if (pid>0)
	{
		/* parent for produce data and send through the pipe */
		// redirect the writeend (data) to the pipe
		close(STDOUT_FILENO);
		dup(fd[WRITEEND]);
		closepipe(fd);
		// 重定向写端之后，向pipe里面传输数据
		for ( i = 2; i <= 35; i++) 
		{
			write(STDOUT_FILENO,&i,sizeof(int));
			// fprintf(STDDER_FILENO,"write into pipe %d\n", i);
		}
		close(STDOUT_FILENO);
		wait(0);

	}else{
		// 子进程用于递归处理
		// 重定向 读的来源
		close(STDIN_FILENO);
		dup(fd[READEND]);
		closepipe(fd);
		// 处理质数
		primes();
	}
	exit(0);
}

