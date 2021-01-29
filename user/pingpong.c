#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDDER_FILENO 2

#define READEND 0
#define WRITEEND 1

typedef int pid_t;

int main(int argc, char *argv[])
{
	// build two pipe
	int fd_1[2];  // parent to child
	int fd_2[2];  // child  to parent

	char byte = 'a';
	// char buf[10];
	pid_t pid;

	pipe(fd_1);
	pipe(fd_2);
	
	pid = fork();
	if (pid < 0)
	{
		fprintf(STDDER_FILENO,"fork error");
		exit(1);
	}

	if(pid ==0)
	{ //child 
		//close p2c write
		close(fd_1[WRITEEND]);
		//close c2p read
		close(fd_2[READEND]);
		
		read(fd_1[READEND],&byte,1);
		printf("%d: received ping\n",getpid());
		
		write(fd_2[WRITEEND],&byte,1);
		close(fd_1[READEND]);
		close(fd_2[WRITEEND]);
	}else{
	  //parent
		close(fd_1[READEND]);
		close(fd_2[WRITEEND]);
	
		// write to child
		write(fd_1[WRITEEND],&byte,1);
		printf("%d: received pong\n",getpid());
		close(fd_1[WRITEEND]);
		close(fd_2[READEND]);
		wait(0);
	}
	exit(0);
}


