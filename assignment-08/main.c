#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("LICENSE");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs);

static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs);

static const struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = &myfd_read,
	.write = &myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

static char str[PAGE_SIZE];
DEFINE_MUTEX(myfd_mutex);

static int __init myfd_init(void)
{
	int retval;

	retval = misc_register(&myfd_device);

	str[0] = 0;

	return retval;
}

static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
}

static ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	ssize_t t, i;
	char *rev_str;
	int retval;
	ssize_t str_size;

	mutex_lock(&myfd_mutex);
	str_size = strlen(str);
	rev_str = kmalloc(str_size + 1, GFP_KERNEL);
	if (!rev_str) {
		mutex_unlock(&myfd_mutex);
		return -ENOMEM;
	}
	for (t = str_size - 1, i = 0; t >= 0; t--, i++)
		rev_str[i] = str[t];
	rev_str[i] = 0x0;
	retval = simple_read_from_buffer(user, size, offs, rev_str, str_size);
	kfree(rev_str);
	mutex_unlock(&myfd_mutex);

	return retval;
}

static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size, loff_t *offs)
{
	ssize_t	res;
	char	*input_buf;
	size_t	input_len;

	if (size + 1 >= PAGE_SIZE)
		return -EINVAL;
	input_buf = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!input_buf)
		return -ENOMEM;

	mutex_lock(&myfd_mutex);

	res = simple_write_to_buffer(input_buf, size, offs, user, PAGE_SIZE - 1);
	input_buf[res] = 0x0;
	input_len = strlen(input_buf);
	if (input_buf[input_len - 1] == '\n')
		input_buf[input_len - 1] = 0;
	strcpy(str, input_buf);

	mutex_unlock(&myfd_mutex);
	kfree(input_buf);

	return res;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
