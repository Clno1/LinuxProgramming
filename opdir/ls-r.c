#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#include<dirent.h>

void read_dir(char *dir);

void isFile(char *name) {
	int ret=0;
	struct stat sb;		//stat结构体

	ret = stat(name,&sb);
	if (ret==-1) {
		perror("stat error");
		return;
	}

	if (S_ISDIR(sb.st_mode)) {	//判断是否目录，是的话继续向下递归
		read_dir(name);
	}

	//不是目录，是文件，打印
	printf("%10s\t\t%ld\n",name,sb.st_size);

	return;
}

void read_dir(char *dir) {
	char path[256];
	DIR* dp;	//目录指针
	struct dirent *sdp;	// 目录项结构体

	dp=opendir(dir);	//open()函数打开目录，返回目录结构体
	if (dp==NULL) {
		perror("opendir error");
		return;
	}

	while ((sdp=readdir(dp))!=NULL) {
		if (strcmp(sdp->d_name,".")==0 || strcmp(sdp->d_name,"..")==0) continue;	//目录里有这两个东西，忽略避免死循环


		//拼接目录字符串并继续递归旧：目录+名字
		sprintf(path,"%s/%s",dir,sdp->d_name);
		isFile(path);
	}

	closedir(dp);	//open了一定记得close
}


int main(int argc,char *argv[])
{
	if (argc==1)
		isFile(".");
	else
		isFile(argv[1]);
	return 0;
}
