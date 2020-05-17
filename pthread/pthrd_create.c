#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

void sys_err(const char *str) {
	perror(str);
	exit(1);
}

//回调函数，创建线程之后就可以在这里工作
void *tfn(void *arg) {
	int i=(int)arg;
	sleep(i);
	printf("I'm %dth thread: pid = %d,tid=%lu\n",i+1,getpid(),pthread_self());
	return NULL;
}

int main(int argc,char *argv[])
{
	int i,ret;
	pthread_t tid;

	for (int i=0;i<5;i++) {
		//参数：线程id，线程属性，回调函数，函数参数
		ret=pthread_create(&tid,NULL,tfn,(void *)i);	//传参用值传递
		if (ret!=0)	//创建线程失败
			sys_err("pthread_create error");
	}

	sleep(i);
	printf("I'm main, pid=%d ,tid=%lu\n",getpid(),pthread_self());
	return 0;
}
