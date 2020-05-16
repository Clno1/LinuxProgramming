#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<errno.h>
#include<pthread.h>
#include<unistd.h>

void sys_err(const char *str) {
	perror(str);
	exit(1);
}

void print_set(sigset_t *set) {
	for (int i=1;i<32;i++) {
		if (sigismember(set,i))
			putchar('1');
		else
			putchar('0');
	}
	printf("\n");
}

int main(int argc,char *argv[])
{
	sigset_t set,oldset,pedset;	//sigset_t是一个bitmap
	int ret=0;

	//以下几行是对set的操作
	sigemptyset(&set);	
	sigaddset(&set,SIGINT);		//屏蔽（阻塞）ctrl+C
	sigaddset(&set,SIGQUIT);
	//sigaddset(&set,SIGBUS);
	//sigaddset(&set,SIGKILL);

	ret=sigprocmask(SIG_BLOCK,&set,&oldset);	//把set设置为阻塞信号，并且把旧的阻塞信号返回给oldset
	if (ret==-1)
		sys_err("sigprocmask error");

	while (1) {	
		ret=sigpending(&pedset);	//取得未决信号集
		print_set(&pedset);		//函数处理未决信号集
		sleep(1);
	}

	return 0;
}
