#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/fs.h>


/*通过seq file 实现内核数据快捷到处到用户层*/


static struct dentry *seq_file_demo_dir;

static int seq_file_demo_show(struct seq_file *seq, void *v)
{
    seq_printf(seq, "hello world\n");
    return 0;
}

static int seq_file_demo_open(struct inode *inode, struct file *file)
{
    return single_open(file, seq_file_demo_show, NULL);
}

static struct file_operations seq_file_demo_fops = {
        .owner = THIS_MODULE,
        .open = seq_file_demo_open,
        .read = seq_read,
        .llseek = seq_lseek,
        .release = seq_release,
};

static int __init seq_file_demo_init(void)
{
    seq_file_demo_dir = debugfs_create_file("seq_file_demo", 0444, NULL, 
                                NULL, &seq_file_demo_fops);
    if (!seq_file_demo_dir) {
        printk("debugfs_create_file failed\n");
        return -1;
    }

    return 0;
}

static void __exit seq_file_demo_exit(void)
{
    if (seq_file_demo_dir) {
        debugfs_remove(seq_file_demo_dir);
    }
}


module_init(seq_file_demo_init);
module_exit(seq_file_demo_exit);


MODULE_AUTHOR("xdd");
MODULE_LICENSE("GPL");



