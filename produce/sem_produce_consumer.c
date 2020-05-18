#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM 5

int queue[NUM];
sem_t blank_number,product_number;

void *producer(void *arg) {
	int i=0;
	while (1) {
		//这里是信号量处理生产过程
		sem_wait(&blank_number);	//空位数信号量--
		queue[i]=rand()%1000+1;	//开始生产
		printf("----Produce----%d\n",queue[i]);
		sem_post(&product_number);	//产品数信号量++

		i=(i+1)%NUM;
		sleep(rand()%1);
	}
}

void *consumer(void *arg) {
	int i=0;
	while (1) {
		//这里是信号量处理消费过程
		sem_wait(&product_number);	//产品数信号量--
		//开始消费
		printf("---consume--%d\n",queue[i]);
		queue[i]=0;
		sem_post(&blank_number);	//空位数信号量++

		i=(i+1)%NUM;
		sleep(rand()%3);
	}
}

int main(int argc,char *argv[])
{
	pthread_t pid,cid;

	//参二0为线程1为进程，参三可以理解为信号量初始值
	sem_init(&blank_number,0,NUM);	//空位数
	sem_init(&product_number,0,0);	//产品数

	//创建生产者消费者线程
	pthread_create(&pid,NULL,producer,NULL);	
	pthread_create(&cid,NULL,consumer,NULL);

	//回收线程
	pthread_join(pid,NULL);
	pthread_join(cid,NULL);

	//销毁信号量
	sem_destroy(&blank_number);
	sem_destroy(&product_number);
	
	return 0;
}
