/*
 * hello world module.
 *
 * Copyright (C) 2018 lcesarz
 */
#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
    printk(KERN_ALERT "Hello World!\n");
    return 0;
}

void cleanup_module(void)
{
	printk(KERN_ALERT "Goodbye, Cruel World!\n");
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luke Cesarz");
MODULE_DSCRIPTION("Just a helloW module");
