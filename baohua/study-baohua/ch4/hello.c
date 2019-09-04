#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 100

int main(void)
{
	int fd, len;
	char str[LENGTH];
	
	fd = open("hello.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	if(fd)
	{
		write(fd, "HELLO TJL", strlen("HELLO TJL"));
		close(fd);
	}
	
	fd = open("hello.txt", O_RDWR);
	len = read(fd, str, LENGTH);
	str[len] = '\0';
	printf("%s\n", str);
	close(fd);
	
	return 0;
}
