/*
 * hello world module.
 *
 * Copyright (C) 2018 lcesarz
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/slab.h>
#include <linux/hid.h>

int hello_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	printk("loading in flashy keyboard.\n");
	return 0;
}

void hello_disconnect(struct usb_interface *intf)
{
	printk("unloading the flashy keyboard.\n");
}

static struct usb_device_id hello_ids[] = {
	{USB_INTERFACE_INFO
	(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
	 USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{},
};

struct usb_driver hello_driver = {
		.name = THIS_MODULE->name,
    		.id_table = hello_ids,
    		.probe = hello_probe,
    		.disconnect = hello_disconnect,
};

int init_module(void)
{
	int result;
    	printk("USB module loaded\n");
	result = usb_register(&hello_driver);
	if (result) 
	    printk(KERN_ALERT "usb_register failed, error %d\n",result);

	return result;
}


void cleanup_module(void)
{
    printk(KERN_ALERT "Tearing down module\n");
    usb_deregister(&hello_driver);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luke Cesarz");
MODULE_DESCRIPTION("Just a helloW module");
