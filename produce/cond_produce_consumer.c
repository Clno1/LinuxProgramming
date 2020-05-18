#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>

//线程不能perror，有自己独特的错误处理方式
void error_thread(int ret,char *str) {
	if (ret!=0) {
		fprintf(stderr,"%s:%s\n",str,strerror(ret));
		pthread_exit(NULL);
	}
}

//把生产的物品做成链表
struct msg{
	int num;
	struct msg *next;
};
struct msg *head;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;	//互斥量
pthread_cond_t has_data=PTHREAD_COND_INITIALIZER;	//条件变量

//生产者线程
void *producer(void *arg) {
	while (1) {
		//生产者生产一个数据
		struct msg *mp=malloc(sizeof(struct msg));
		mp->num = rand()%1000+1;
		printf("--produce %d\n",mp->num);
		
		//利用互斥量对公共区域做互斥
		pthread_mutex_lock(&mutex);	//加互斥锁
		mp->next=head;			//写公共区域
		head=mp;
		pthread_mutex_unlock(&mutex);	//解互斥锁

		//条件变量：唤醒阻塞在条件变量has_data的线程
		pthread_cond_signal(&has_data);

		sleep(rand()%3);
	}
	return NULL;
}

//消费者线程
void *consumer(void *arg) {
	while (1) {
		struct msg *mp;

		//
		pthread_mutex_lock(&mutex);
		while (head==NULL) 
			pthread_cond_wait(&has_data,&mutex);

		//消费生产品，读写公共区域
		mp=head;
		head=mp->next;
		printf("-------------consumer id %lu :%d\n",pthread_self(),mp->num);
		free(mp);
		
		sleep(rand()%3);
	}
	return NULL;
}

int main(int argc,char *argv[]) {
	int ret;
	pthread_t pid,cid;

	srand(time(NULL));

	ret=pthread_create(&pid,NULL,producer,NULL);		//生产者
	if (ret!=0)
		err_thread(ret,"pthread_create producer error");

	ret=pthread_create(&cid,NULL,consumer,NULL);		//消费者
	if (ret!=0)
		err_thread(ret,"pthread_create producer error");

	return 0;
}
