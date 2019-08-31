/*************************************************************************
    > File Name: netlink-1.c
    > Author: tlosky
    > Mail: tlosky@email.com
    > Created Time: 2019年08月30日 星期五 23时39分47秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <linux/netlink.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>

static void die(char *s)
{
    write(2, s, strlen(s));
    exit(1);
}

int main(int argc, char *argv[])
{
    struct sockaddr_nl nls;
    struct pollfd pfd;
    char buf[512];

    //打开hotplug事件的netlink套接字
    memset(&nls, 0, sizeof(struct sockaddr_nl));
    nls.nl_family = AF_NETLINK;
    nls.nl_pid = getpid();
    nls.nl_groups = -1;

    pfd.events = POLLIN;
    pfd.fd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (pfd.fd == -1)
        die("Not root\n");
    //监听netlink套接字
    if (bind(pfd.fd, (void *)&nls, sizeof(struct sockaddr_nl)));
        die("Bind failed\n");
    while(-1 != poll(&pfd, 1, -1)){
        int i, len = recv(pfd.fd, buf, sizeof(buf), MSG_DONTWAIT);
        if (len == -1)
            die("recv\n");

        //打印数据到标准输出
        i = 0;
        while(i < 10){
            printf("%s\n", buf+i);
            i += strlen(buf + i) + 1;
        }
    }
    die("poll\n");

    return 0;
}


