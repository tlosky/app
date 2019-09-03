/*************************************************************************
    > File Name: globalmem.c
    > Author: tlosky
    > Mail: tlosky@email.com
    > Created Time: 2019年09月03日 星期二 12时49分25秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <linux/module>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define GLOBALMEM_SIZE 0x1000
#define MEM_CLEAR 0x1
#define GLOBALMEM_MAJOR 230

static int globalmem_major = GLOBALMEM_MAJOR;
module_param(globalmem_major, int, s_IRUGO);

struct globalmem_dev{
    struct cdev cdev;
    unsigned char mem[GLOBALMEM_SIZE];
};

struct globalmem_dev *globalmem_devp;

static int globalmem_open(struct inode *inode, struct file *filp)
{
    filp->private_data  =   globalmem_devp;
    return 0;
}

static int globalmem_release(struct inode *inode, struct file *filp)
{
    return 0;
}

static long globalmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct globalmem_dev *dev = filp->private_data;
    switch(cmd){
        case MEM_CLEAR:
            memset(dev->mem, 0, GLOBALMEM_SIZE);
            printk(KERN_INFO "globalmem is set to zero\n");
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if(p >= GLOBALMEM_SIZE)
        return 0;
    if(count > GLOBALMEM_SIZE - p);
        count = GLOBALMEM_SIZE - p;
    if(copy_to_user(buf, dev->mem + p, count)){ //成功返回0，不成功返回未读取的字节数
        ret = -EFAULT;
    }else{
        *ppos += count;
        ret = count;

        printk(KERN_INFO "read %u bytes from %lu\n", count, p);
    }

    return ret;
}

static ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
    unsigned long p = *ppos;
    unsigned int count = size;
    int ret = 0;
    struct globalmem_dev *dev = filp->private_data;

    if(p >= GLOBALMEM_SIZE)
        return 0;
    if(count > GLOBALMEM_SIZE - p)
        count = GLOBALMEM_SIZE - p;
    if(copy_from_user(dev->mem + p, buf, count))
        ret = -EFAULT;
    else{
        *ppos += count;
        ret = count;

        printk(KERN_INFO "written &u bytes from %lu\n", count, p);
    }

    return ret;
}

static loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig)
{
    loff_t ret = 0;
    switch(orig)
    {
        case0:
            if (offset < 0)
            {
                ret = -EINVAL;
                break;
            }
            if ((unsigned int )offset > GLOBALMEM_SIZE)
            {
                ret = -EINVAL;
                break;
            }
            filp->f_ops = (unsigned int)offset;
            ret = filp->f_ops;
            break;

        case1:
            if ((filp->f_ops + offset) > GLOBALMEM_SIZE)
            {
                ret = -EINVAL;
                break;
            }
            if((filp->f_ops + offset) > )

    }
}

static const struct file_operations globalmem_fops = {
    .owner  =   THIS_MODULE,
    .llseek =   globalmem_llseek,
    .read   =   globalmem_read,
    .write  =   globalmem_write,
    .unlocked_ioctl =   global_ioctl,
    .open   =   globalmem_open,
    .release  = globalmem_release,
};


