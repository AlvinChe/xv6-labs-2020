#include "kernel/types.h"
#include "kernel/param.h"
// kernel/param.h declares MAXARG
#include "user/user.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDDER_FILENO 2

#define MAXLINE 1024

int main(int argc, char  *argv[])
{
	if (argc <2){
		fprintf(STDDER_FILENO,"usage: xargs command\n" );
		exit(1);
	}
	if (argc>MAXARG)
	{
		fprintf(STDDER_FILENO,"too much xargs command\n" );
		exit(1);
	}

	char line[MAXLINE];
	char *param[MAXARG];
	int n,argc_index = 0;
	// int i;


	char *cmd = argv[1];

	// 先处理自己带的参数命令，存放到数组中
	for (int i = 1; i < argc; ++i)
	{
		param[argc_index++]=argv[i];
	}
	
	// 处理从管道送过来的参数
	while((n = read(STDIN_FILENO,line,MAXLINE) )>0){
		if (fork() == 0)
		{
			// 每条命令最大长度为1024
			char *arg = (char*)malloc(sizeof(line));
			int index = 0;
			for (int i = 0; i < n; ++i)
			{
				// 遇到空格和回车表明是一条命令
				if (line[i] == ' ' || line[i] == '\n')
				{
					arg[index] = 0;
					param[argc_index++] = arg;
					index = 0;
					arg = (char*) malloc(sizeof(line));
				}else{
					arg[index++] = line[i];
				}
			}
			arg[index] = 0;
			param[argc_index] = 0;
			// 处理完所有的字符串，开始由exec来执行
			exec(cmd,param);
		}else{
			// 父进程等待执行结果
			wait(0);
		}
	}
	exit(0);
}