#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include<errno.h>
#include<pthread.h>

void sys_err(const char *str) {
	perror(str);
	exit(1);
}

void catch_child(int signo) {
	pid_t wpid;
	int status;

	//这里的while非常重要，当接受到SIGCHLD信号时候，有多个子进程同时死亡，这时候就需要while来把这段时间的死亡子全部回收
	while ((wpid=waitpid(-1,&status,0))!=-1) {
		if (WIFEXITED(status))
			printf("-------------catch child id %d, ret=%d \n",wpid,WEXITSTATUS(status));
	}
	return;
}

int main(int argc,char *argv[])
{
	pid_t pid;
	int i;

	for (i=0;i<15;i++)
		if ((pid=fork())==0)	//创建15个子进程
			break;

	if (i==15) {	//父进程
		
		//信号结构体，三个参数重要
		struct sigaction act;	

		act.sa_handler=catch_child;	//注册函数
		sigemptyset(&act.sa_mask);	//在执行期间，sa_mask会替换原mask
		act.sa_flags=0;		//设为0，在该信号处理函数期间，再次收到同样信号就屏蔽

		sigaction(SIGCHLD,&act,NULL);

		printf("I'm parent, pid=%d \n",getpid());

		while(1);

	} else {	//子进程
		printf("I'm child pid= %d\n",getpid());
		return i;
	}
	return 0;
}
