#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<errno.h>

struct student{
	int id;
	char name[256];
	int age;
};

void sys_err(const char *str) {
	perror(str);
	exit(1);
}

int main(int argc,char *argv[])
{
	struct student stu={1,"xiaoming",18};
	struct student *p;
	int fd;

	//写进程
	fd=open("test_map",O_RDWR);
	if (fd==-1)
		sys_err("open error");
	//这个进程负责创建磁盘文件，一开始要往里面填点东西
	ftruncate(fd,sizeof(stu));

	//写进程，以读写方式打开i
	p=mmap(NULL,sizeof(stu),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	if (p==MAP_FAILED)
		sys_err("mmap error");
	close(fd);

	//不断循环，周期性向mmap里面写东西
	while (1) {
		memcpy(p,&stu,sizeof(stu));
		stu.id++;
		sleep(2);
	}

	munmap(p,sizeof(stu));

	return 0;
}
