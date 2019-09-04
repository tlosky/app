#include <stdio.h>

#define LENGTH 100

int main(void)
{
	FILE *fd;
	char str[LENGTH];

	fd = fopen("hello-1.txt", "w+");//创建并打开文件
	if(fd){
		fputs("hello tjl", fd); //写入字符串
		fclose(fd);
	}

	fd = fopen("hello-1.txt", "r");
	fgets(str, LENGTH, fd); //读取文件内容
	printf("%s\n", str);
	fclose(fd);

	return 0;

}
