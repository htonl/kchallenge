/*
 * hello world module.
 *
 * Copyright (C) 2018 lcesarz
 */
#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
	static struct usb_device_id hello_ids[] = {
	    {.driver_info = 42},
	    {}
	};

	static struct usb_driver hello_driver = {
	    .owner = THIS_MODULE,
	    .name = "Flashy keyboard",
	    .id_table = hello_ids,
	    .probe = hello_probe,
	    .disconnect = hello_disconnect,
	};
	return 0;
}

int hello_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	printk("loading in flashy keyboard.\n");
	return 0;
}

void disconnect(struct usb_interface *intf)
{
	printk("unloading the flashy keyboard.\n");
}

void cleanup_module(void)
{
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luke Cesarz");
MODULE_DSCRIPTION("Just a helloW module");
