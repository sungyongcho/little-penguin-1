#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sungyong Cho");
MODULE_DESCRIPTION("little-penguin-1 Assignment 01");

static int __init hello_world_init(void)
{
    printk(KERN_INFO "Hello world !\n");
    return 0;
}

static void __exit hello_world_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_world_init);
module_exit(hello_world_cleanup);
