#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<pthread.h>

int main(int argc,char *argv[])
{
	int i;
	pid_t pid,wpid;

	for (i=0;i<5;i++) {
		pid=fork();
		if (pid==0) break;
	}

	if (i==5) {	//父进程
		
		//这里不断while循环等待死亡的子进程
		while ((wpid=waitpid(-1,NULL,WNOHANG))!=-1) {
			if (wpid>0) {	//回收成功
				printf("wait child %d \n",wpid);
			} else if (wpid==0) {	//回收失败
				sleep(1);	//sleep一秒之后再尝试回收
				continue;
			}
		}
	} else {	//5个子进程
		sleep(i);	//子进程sleep以一下
		printf("I'm child %d ,i am dying",i);
	}
	return 0;
}
