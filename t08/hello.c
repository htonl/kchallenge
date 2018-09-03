/*
 * hello world module.
 * Copyright (C) 2018 lcesarz
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/debugfs.h>
#include <linux/semaphore.h>

#define MESSAGE "Lucas Pukas"
#define MESSAGE_SIZE sizeof(MESSAGE)
#define JIFFIES "jiffies"
#define ID_FILE "id"
#define EUDY_DIR "eudyptula"
static DEFINE_SEMAPHORE(sem);
struct dentry *id_d;
struct dentry *jiffies_d;
struct dentry *eudy_d;
struct dentry *foo_d;
static char *foo_buf;
static int foo_len;


static ssize_t id_read(struct file *filp, char __user *buf, size_t count,
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

static ssize_t id_write(struct file *filp, const char __user *buf, size_t count,
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

static const struct  file_operations id_fops = {
    .owner = THIS_MODULE,
    .read = id_read,
    .write = id_write
};

static ssize_t foo_read(struct file *filp, char __user *buf, size_t count,
	loff_t *fpos)
{
	if (*fpos != 0)
	    return 0;
	down(&sem);
	if (copy_to_user(buf, foo_buf, foo_len))
		return -EINVAL;

	*fpos += count;
	up(&sem);
	return count;
}

static ssize_t foo_write(struct file *filp, const char __user *buf, 
	size_t count, loff_t *fpos)
{
    if (count >= PAGE_SIZE)
	return -EINVAL;
    down(&sem);
    if( copy_from_user(foo_buf, buf, count)){
	foo_len = 0;
	return -EINVAL;
    }
    else {
	foo_len = count;
    }
    up(&sem);
    return count;
}

static const struct  file_operations foo_fops = {
    .owner = THIS_MODULE,
    .read = foo_read,
    .write = foo_write
};

int init_module(void)
{
	const char* fname = ID_FILE;
	const char* dirname = EUDY_DIR;
	const char* jiffies_name = JIFFIES;
	const char* fooname = "foo";

	eudy_d = debugfs_create_dir(dirname, NULL);
	if (eudy_d == NULL)
	    return -ENOMEM;
	pr_debug("created debugfs dir %s\n",dirname);

	jiffies_d = debugfs_create_u64(jiffies_name, S_IRWXUGO, eudy_d, 
		(u64*)&jiffies);
	if(jiffies_d == NULL)
	    return -ENOMEM;
	pr_debug("created debugfs dir %s\n",dirname);

	id_d = debugfs_create_file(fname, S_IRWXUGO, eudy_d, NULL, 
		&id_fops);
	if (id_d == NULL)
	    return -ENOMEM;
	pr_debug("created debugfs file%s\n",fname);

	foo_d = debugfs_create_file(fooname, S_IRWXUGO, eudy_d, NULL,
		&foo_fops);
	if (foo_d == NULL)
	    return -ENOMEM;
	pr_debug("created debugfs file%s\n",fooname);

	foo_buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if(!foo_buf)
	    return -ENOMEM;

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_ALERT "Freeing everything\n");
	debugfs_remove(id_d);
	debugfs_remove(jiffies_d);
	debugfs_remove(foo_d);
	debugfs_remove(eudy_d);
	kfree(foo_buf);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luke Cesarz");
MODULE_DESCRIPTION("Just a hello module");
