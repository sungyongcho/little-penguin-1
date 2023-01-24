#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sungyong Cho");
MODULE_DESCRIPTION("little-penguin-1 Assignment 07");

#define LOGIN "sucho"
#define LOGIN_LEN 5

static struct dentry *fortytwo_dir;
static struct dentry *id_file;
//static struct dentry *jiffies_file;
//static struct dentry *foo_file;

static int	id_open(struct inode *inode, struct file *file);
static ssize_t	id_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t	id_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.open = id_open,
	.read = id_read,
	.write = id_write
};

static int	id_open(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t id_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	char id_string[LOGIN_LEN] = LOGIN;
	return simple_read_from_buffer(buf, count, offset, id_string, strlen(id_string));
}

static ssize_t id_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	char id_string[LOGIN_LEN];
	int ret = simple_write_to_buffer(id_string, LOGIN_LEN, offset, buf, count);

	if (ret < 0)
		return ret;
	id_string[ret] = '\0';
	if (strncmp(id_string, LOGIN, LOGIN_LEN) == 0)
		return count;
	return -EINVAL;
}

static int __init hello_world_init(void)
{
	fortytwo_dir = debugfs_create_dir("fortytwo", NULL);
	id_file = debugfs_create_file("id", 0666, fortytwo_dir, NULL, &id_fops);
	//chown -R 0:0 /sys/kernel/debug/fortytwo;
	printk(KERN_INFO "debugfs - Hello world !\n");
	return 0;
}

static void __exit hello_world_cleanup(void)
{
	debugfs_remove_recursive(fortytwo_dir);

	printk(KERN_INFO "debugfs - Cleaning up module.\n");
}






module_init(hello_world_init);
module_exit(hello_world_cleanup);
