#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/wait.h>

int var=100;

int main(int argc,char *argv[])
{
	int *p;
	pid_t pid;

	int fd;
	fd=open("temp",O_RDWR|O_CREAT|O_TRUNC,0644);	//mmap得现有磁盘文件
	if (fd<0) {
		perror("open error");
		exit(1);
	}
	ftruncate(fd,4);	//将参数fd指定的文件大小改为参数指定的大小,这样做是因为mmap的大小依赖于磁盘文件大小


	//建立映射区，参数：大小，读写权限，共享/私有，依靠的文件
	//这里的private意味着mmap归父进程所有，不能用于子进程通信
	//p=(int *)mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
	p=(int *)mmap(NULL,4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if (p==MAP_FAILED) {	//mmap建立失败
		perror("mmap error");
		exit(1);
	}
	close(fd);	//建立完之后mmap就不依靠文件了，尽早关闭掉


	pid=fork();
	if (pid==0) {
		*p=7000;	//写mmap
		var=1000;	//父子进程之间copy on write
		printf("child ,*p=%d, var=%d\n",*p,var);	//读mmap
	} else {
		sleep(1);
		printf("parent, *p=%d, var=%d\n",*p,var);	//读mmap
		wait(NULL);
		
		//释放mmap，参数是：首地址和大小
		int ret=munmap(p,4);
		if (ret==-1) {
			perror("munmap error");
			exit(1);
		}
	}

	return 0;
}
