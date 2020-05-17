#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

//互斥锁设为全局变量
pthread_mutex_t mutex;

//子线程
void *tfn(void *arg) {
	srand(time(NULL));
	while (1) {
		pthread_mutex_lock(&mutex);	//加锁，不行则阻塞
		printf("hello ");
		printf("world\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand()%3);
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	srand(time(NULL));
	
	pthread_mutex_init(&mutex,NULL);	//初始化互斥锁

	pthread_create(&tid,NULL,tfn,NULL);	//创造子线程

	//主线程
	while (1) {
		pthread_mutex_lock(&mutex);	//互斥锁加锁，不行则阻塞
		printf("HELLO ");
		printf("WORLD\n");
		pthread_mutex_unlock(&mutex);	//互斥锁解锁
		sleep(rand()%3);
	}
	
	//最后记得回收子线程，销毁互斥锁
	pthread_join(tid,NULL);
	pthread_mutex_destroy(&mutex);
	return 0;
}
