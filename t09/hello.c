#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/semaphore.h>
#include <linux/slab.h>
#include <linux/jiffies.h>

#define MSG "Luke is king"
#define MSG_SIZE sizeof(MSG)

static DEFINE_SEMAPHORE(sem);
static char* foo_buf;
static int foo_size;

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	down(&sem);
	strncpy(buf, foo_buf, foo_size);
	up(&sem);
	return foo_size;
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
	if(count >= PAGE_SIZE)
	{
		foo_size = 0;
		return -EINVAL;
	}
	down(&sem);
	strncpy(foo_buf, buf, count);
	foo_size = count;
	up(&sem);
	return count;
}

static struct kobj_attribute foo_attribute = 
	__ATTR(foo, 0664, foo_show, foo_store);

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
	const char* tmp= MSG;
	strncpy(buf, tmp, MSG_SIZE);
	return MSG_SIZE;
}

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
	if(strncmp(buf, MSG, MSG_SIZE - 1))
		return -EINVAL;
	else
		return count;
}

static struct kobj_attribute id_attribute =
        __ATTR(id, 0664, id_show, id_store);


static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
                        char *buf)
{
	return sprintf(buf, "%lu\n", jiffies);
}

static ssize_t jiffies_store(struct kobject *kobj, struct kobj_attribute *attr,
                         const char *buf, size_t count)
{
	return -EINVAL;
}

static struct kobj_attribute jiffies_attribute =
        __ATTR(jiffies, 0664, jiffies_show, jiffies_store);

static struct kobject *eudy_kobj;
static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&id_attribute.attr,
	&jiffies_attribute.attr,
	NULL,
};
static struct attribute_group attr_group = {
	.attrs = attrs,
};

int init_module(void)
{
	int retval = -1;

	eudy_kobj = kobject_create_and_add("eudy_kobj", kernel_kobj);
	if (!eudy_kobj)
		goto mem_fail;
	
	retval = sysfs_create_group(eudy_kobj, &attr_group);
	if (retval)
		kobject_put(eudy_kobj);

	foo_buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!foo_buf)
		goto mem_fail;

mem_fail: pr_alert("Could not create devices");
	
	return retval;
}

void cleanup_module(void)
{
	pr_alert("putting kobj, and freeing foo_buf\n");
	kobject_put(eudy_kobj);
	kfree(foo_buf);
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luke Cesarz");
MODULE_DESCRIPTION("Simple Sysfs module");
