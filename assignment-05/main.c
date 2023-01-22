#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sungyong Cho");
MODULE_DESCRIPTION("little-penguin-1 Assignment 05");

#define LOGIN "sucho"
#define LOGIN_LEN 5

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *offset);
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *offset);

static struct file_operations my_fops = {
	.read = my_read,
	.write = my_write,
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &my_fops,
};

static int __init hello_world_init(void)
{
	int ret;
	ret = misc_register(&my_misc_device);

	if (ret)
	{
		printk(KERN_ERR "Failed to register the misc device: %d\n", ret);
	}
	
	printk(KERN_INFO "Misc device - registered\n");
	return 0;
}

static void __exit hello_world_cleanup(void)
{
	misc_deregister(&my_misc_device);
	
	printk(KERN_INFO "Misc device - Cleaning up module.\n");
}

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
	char my_string[LOGIN_LEN] = LOGIN;
	return simple_read_from_buffer(buf, len, offset, my_string, LOGIN_LEN);
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
	char my_string[LOGIN_LEN];
	int ret = simple_write_to_buffer(my_string, LOGIN_LEN, offset, buf, len);

	if (ret < 0)
	{
		return ret;
	}
	my_string[ret] = '\0';
	if (strncmp(my_string, LOGIN, LOGIN_LEN) == 0)
	{
		return len;
	}
	return -EINVAL;
}


module_init(hello_world_init);
module_exit(hello_world_cleanup);
