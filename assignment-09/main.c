#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/string.h>

#include <linux/mount.h>
#include <linux/proc_fs.h>
#include <linux/nsproxy.h>
#include <linux/slab.h>
#include <../fs/mount.h>

MODULE_LICENSE("LICENSE");
MODULE_AUTHOR("Sungyong Cho <sucho@student.42.fr>");
MODULE_DESCRIPTION("little-penguin-1 Assignment 09");

static struct proc_dir_entry *mymounts;

static int set_path(struct mount *mnt, char **buffer, int *i)
{
	char path[256];

	if (strcmp(mnt->mnt_parent->mnt_mountpoint->d_name.name, "/") != 0)
		set_path(mnt->mnt_parent, buffer, i);
	if (strcmp(mnt->mnt_mountpoint->d_name.name, "/") != 0)
		*i += snprintf(*buffer + *i, PAGE_SIZE - *i, "%s", dentry_path_raw(mnt->mnt_mountpoint, path, sizeof(path)));
	else
		*i += snprintf(*buffer + *i, PAGE_SIZE - *i, "\t%.*s", (int)strlen(dentry_path_raw(mnt->mnt_mountpoint, path, sizeof(path))) - 1, dentry_path_raw(mnt->mnt_mountpoint, path, sizeof(path)));

	return *i;
}

static ssize_t mymounts_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos)
{
    char *buffer;
    struct mount *mnt;
    char path[256];
    int ret = 0;
    int i = 0;

    buffer = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (!buffer)
        return -ENOMEM;

    list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list) {
        if (strcmp(mnt->mnt_devname, "none") == 0)
            continue;
        memset(path, 0, sizeof(path));
        if (strcmp(mnt->mnt_devname, "/dev/root") == 0) {
            i += snprintf((&(buffer[i])), PAGE_SIZE - i - 1, "%-1s", "root");
        } else if (strcmp(mnt->mnt_mountpoint->d_name.name, "/") == 0) {
            continue;
        } else {
            i += snprintf((&(buffer[i])), PAGE_SIZE - i, "%s", mnt->mnt_mountpoint->d_name.name);
        }
        set_path(mnt->mnt_parent, &buffer, &i);
        i += snprintf((&(buffer[i])), PAGE_SIZE - i, "\t%s\n", dentry_path_raw(mnt->mnt_mountpoint, path, sizeof(path)));
    }
    ret = simple_read_from_buffer(buf, count, f_pos, buffer, strlen(buffer));
    kfree(buffer);
    return ret;
}

struct proc_ops mymounts_ops = {
	.proc_read = mymounts_read,
};

int __init mymounts_init(void)
{
	mymounts =proc_create("mymounts", 0660, NULL, &mymounts_ops);
	return 0;
}

void __exit mymounts_exit(void)
{
	proc_remove(mymounts);
}

module_init(mymounts_init);
module_exit(mymounts_exit);

