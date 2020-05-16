#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	pid_t pid,wpid;
	int status;

	pid=fork();
	if (pid==0) {	//子进程
		printf("i am child, pid is %d, sleep 10s ",pid);
		sleep(20);
		printf("i am diying");
		return 73;
	} else if (pid>0) {	//父进程
		//wait函数会阻塞，如果没有一个子进程死亡
		//wpid=wait(NULL);	//不关心子进程如何死亡
		wpid=wait(&status);	//关心子进程死亡，写到status里面
		if (wpid==-1) {
			perror("wait error");
			exit(1);
		}

		if (WIFEXITED(status))		//为真，子进程正常终止
			printf("child exit with %d\n",WEXITSTATUS(status));
		
		if (WIFSIGNALED(status))	//为假，子进程被信号终止
		       printf("child kill with signal %d\n",WTERMSIG(status));

		printf("wait successful, %d died",pid);	       

	} else {
		perror("fork error");	//fork出错了
		return 1;
	}
	return 0;
}
