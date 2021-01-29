#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char* args[])
{
	if(argc!=2){
		fprintf(0,"usage: spleep <number>\n");
		exit(1);
	}
	int number = atoi(args[1]);
	sleep(number);
	exit(0);
}
