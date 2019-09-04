#########################################################################
# File Name: sysfs.sh
# Author: tlosky
# mail: tlosky@email.com
# Created Time: 2019年08月31日 星期六 22时05分27秒
#########################################################################
#!/bin/bash

#sysfs虚拟文件系统产生所有硬件的层级视图
#proc文件系统提供进程和状态信息
#该脚本遍历sysfs，找出所有的设备，并分析打印出设备名和主次设备号

#block devices
echo block devices:
for i in /sys/block/*/dev /sys/block/*/*/dev
do
    if [ -f $i ]
    then
        MAJOR=$(sed 's/:.*//' < $i)
        MINOR=$(sed 's/.*://' < $i)
        DEVNAME=$(echo $i | sed -e 's@/dev@@' -e 's@.*/@@')
        echo /dev/$DEVNAME b $MAJOR $MINOR
        #mknod /dev/$DEVNAME b $MAJOR $MINOR
    fi
done
echo 
echo 

#char devices
echo char devices
for i in /sys/bus/*/devices/*/dev /sys/class/*/*/dev
do
    if [ -f $i ]
    then
        MAJOR=$(sed 's/:.*//' < $i)
        MINOR=$(sed 's/.*://' < $i)
        DEVNAME=$(echo $i | sed -e 's@/dev@@' -e 's@.*/@@')
        echo /dev/$DEVNAME c $MAJOR $MINOR
        #mknod /dev/$DEVNAME c $MAJOR $MINOR
    fi
done
