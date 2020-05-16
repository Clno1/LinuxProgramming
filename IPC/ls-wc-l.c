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

int main(int argc,char *argv[])
{
	int fd[2];
	int ret;
	pid_t pid;

	ret=pipe(fd);		//创建管道
	if (ret==-1) sys_err("pipe error");


	pid=fork();		//创建子进程
	
	//父子进程合作完成ls wc -l
	if (pid==-1) 		//失败
		sys_err("fork error");	
	else if (pid>0) {	//父进程完成wc -l
		close(fd[1]);	//父进程读，关闭写端
		dup2(fd[0],STDIN_FILENO);	//有读端且为空，pipe会阻塞到读到数据
		execlp("wc","wc","-l",NULL);
		sys_err("exclp wc error");	//一旦执行了execlp就结束了，没法走到这里，所以这里可以用来判错
	} else if (pid==0) {	//子进程完成ls
		close(fd[0]);	//子进程写，关闭读端
		dup2(fd[1],STDOUT_FILENO);
		execlp("ls","ls",NULL);
		sys_err("exclp ls error");
	}	
	return 0;
}
