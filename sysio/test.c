#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>

void error_handling(char *message);

//实现把argv[1]文件内容copy到argv[2]文件中
int main(int argc,char *argv[])
{
	char buf[1];
	int n=0;
	
	//open函数打开argv[1]文件并返回文件描述符
	int fd1=open(argv[1],O_RDONLY);
	if (fd1==-1) error_handling("open argv1 error");

	//同上
	int fd2=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0664);
	if (fd2==-1) error_handling("open argv2 error");

	//copy主体
	while ((n=read(fd1,buf,1024))!=0) {	//read函数从fd1读取文件到buf，并返回读取大小
		if (n<0) {	//读完了
			perror("read error");
			break;
		}
		write(fd2,buf,n);	//把读取到的buf的内容写到fd2中
	}

	close(fd1);	//关闭fd1
	close(fd2);	//关闭fd2

	return 0;
}

void error_handling(char *message) {
	perror(message);
	exit(1);
}
