#include <linux/module.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

MODULE_LICENSE("GPL");

static int mymounts_open(struct inode *inode, struct file *file);

static int mymounts_show(struct seq_file *m, void *v) {
	    struct path root;
	        struct vfsmount *mnt;
		    seq_printf(m, "Mount Point\tName\n");
		        seq_printf(m, "----------\t----\n");
			    for (mnt = current->fs->root.mnt; mnt; mnt = next_mnt(mnt, &root)) {
				            seq_printf(m, "%s\t%s\n", mnt->mnt_root->d_name.name, mnt->mnt_mountpoint->d_name.name);
					        }
			        return 0;
}

static int mymounts_open(struct inode *inode, struct file *file) {
	    return single_open(file, mymounts_show, NULL);
}

static const struct proc_ops	mymounts_fops ={
	.open = mymounts_open
};

static int __init mymounts_init(void) {
	proc_create("mymounts", 0, NULL, &mymounts_fops);
	return 0;
}

static void __exit mymounts_exit(void) {
	remove_proc_entry("mymounts", NULL);
}

module_init(mymounts_init);
module_exit(mymounts_exit);
