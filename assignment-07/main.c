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
static struct dentry *jiffies_file;
static struct dentry *foo_file;

static char *foo_string = NULL;
static size_t foo_len;
static DEFINE_MUTEX(foo_mutex);

static int	id_open(struct inode *inode, struct file *file);
static ssize_t	id_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t	id_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);
static int	jiffies_show(struct seq_file *m, void *v);
static int	jiffies_open(struct inode *inode, struct file *file);
static ssize_t	foo_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t	foo_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);

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

static ssize_t	id_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	char id_string[LOGIN_LEN] = LOGIN;
	return simple_read_from_buffer(buf, count, offset, id_string, strlen(id_string));
}

static ssize_t	id_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
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

static const struct file_operations 	jiffies_fops = {
	.owner = THIS_MODULE,
	.open = jiffies_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release
};

static int	jiffies_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%lu\n", jiffies);
	return 0;
}

static int	jiffies_open(struct inode *inode, struct file *file)
{
	return single_open(file, jiffies_show, NULL);
}

static const struct file_operations	foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

static ssize_t	foo_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	ssize_t	ret;

	mutex_lock(&foo_mutex);

	if (foo_string == NULL)
	{
		mutex_unlock(&foo_mutex);
		return 0;
	}

	if (*offset >= foo_len)
	{
		mutex_unlock(&foo_mutex);
		return 0;
	}

	if (count > (foo_len - *offset))
		count = foo_len - *offset;

	ret = simple_read_from_buffer(buf, count, offset, foo_string, strlen(foo_string));
	mutex_unlock(&foo_mutex);

	return ret;
}

static ssize_t	foo_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
	char *local = NULL;
	ssize_t	ret;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	if (count > PAGE_SIZE)
		return -EINVAL;

	local = kmalloc(count, GFP_KERNEL);

	ret = simple_write_to_buffer(local, count, offset, buf, count);

	if (ret < 0)
	{
		kfree(local);
		return ret;
	}

	mutex_lock(&foo_mutex);

	kfree(foo_string);
	foo_string = local;
	foo_len = count;

	mutex_unlock(&foo_mutex);

	return ret;
}

static int __init	hello_world_init(void)
{
	fortytwo_dir = debugfs_create_dir("fortytwo", NULL);
	id_file = debugfs_create_file("id", 0666, fortytwo_dir, NULL, &id_fops);
	jiffies_file = debugfs_create_file("jiffies", 0444, fortytwo_dir, NULL, &jiffies_fops);
	foo_file = debugfs_create_file("foo", 0600, fortytwo_dir, NULL, &foo_fops);
	printk(KERN_INFO "debugfs - Hello world !\n");
	return 0;
}

static void __exit	hello_world_cleanup(void)
{
	debugfs_remove_recursive(fortytwo_dir);
	if (foo_string != NULL)
		kfree(foo_string);
	printk(KERN_INFO "debugfs - Cleaning up module.\n");
}

module_init(hello_world_init);
module_exit(hello_world_cleanup);
