/*
 * hello world module.
 *
 * Copyright (C) 2018 lcesarz
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/errno.h>

#define MESSAGE "Lucas Pukas"
#define MESSAGE_SIZE sizeof(MESSAGE)

static ssize_t hello_read(struct file *filp, char __user *buf, size_t count,
	loff_t *fpos)
{
	char *str = MESSAGE;

	if (*fpos != 0)
	    return 0;

	if (count < MESSAGE_SIZE || copy_to_user(buf, str, MESSAGE_SIZE))
		return -EINVAL;

	*fpos += count;
	return count;
}

static ssize_t hello_write(struct file *filp, const char __user *buf, size_t count,
	loff_t *fpos)
{
    char *str = MESSAGE;
    char input[MESSAGE_SIZE];

    if (count != MESSAGE_SIZE || copy_from_user(input, buf, count) || 
	    strncmp(str, input, MESSAGE_SIZE -1))
	return -EINVAL;
    else
	return count;
}


static const struct  file_operations hello_fops = {
    .owner = THIS_MODULE,
    .read = hello_read,
    .write = hello_write
};

struct miscdevice hello_device = {
    MISC_DYNAMIC_MINOR,
    THIS_MODULE->name,
    &hello_fops,
};

int init_module(void)
{
	printk(KERN_ALERT "Hello World!\n");
	return misc_register(&hello_device);
}

void cleanup_module(void)
{
	printk(KERN_ALERT "Goodbye, Cruel World!\n");
	misc_deregister(&hello_device);
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luke Cesarz");
MODULE_DESCRIPTION("Just a helloW module");
