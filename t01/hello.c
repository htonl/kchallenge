/*
 * =====================================================================================
 *
 *       Filename:  hello.c
 *
 *    Description:  Hello Worled kernel module
 *
 *        Version:  1.0
 *        Created:  08/25/2018 03:00:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Luke Cesarz
 *
 * =====================================================================================
 */

#include <linux/module.h>
#include <linux/kernel.h>
MODULE_LICENSE("gpl v2");

int init_module(void)
{
    printk(KERN_ALERT "Hello World!\n");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_ALERT "Goodbye, Cruel World!\n");
}
