#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
/*
	用户层读写proc文件来获取内核中的某个变量值
	以此来达到用户态和内核态交互的目的
*/
#define KSTRING_LEN 30
static struct proc_dir_entry *proc_num;
static int number = 10;
static char kstring[KSTRING_LEN];

static int proc_test_open(struct inode *inode, struct file *file)
{
	return 0;
}

static 
	ssize_t proc_test_read(struct file *file, char __user *buffer, size_t length, loff_t *ppos)
{
	int nbytes = sprintf(kstring, "%d\n", number);
	return simple_read_from_buffer(buffer, length, ppos, kstring, nbytes);
}

static	
	ssize_t proc_test_write(struct file *file, const char __user *buffer, size_t length, loff_t *ppos)
{
	ssize_t ret_val;
	ret_val = simple_write_to_buffer(kstring, KSTRING_LEN, ppos, buffer, length);
	if (ret_val < 0) {
		printk("simple_write_to_buffer failed\n");
		return -1;
	}

	sscanf(kstring, "%d", &number);
	printk("number is be set to %d\n", number);

	return ret_val;
}


static struct file_operations proc_fops = {
	.owner = THIS_MODULE,
	.open = proc_test_open,
	.read = proc_test_read,
	.write = proc_test_write,
};


static int __init my_module_init(void)
{
	proc_num = proc_create("my_module_num", 666, NULL, &proc_fops);
	if (proc_num == NULL) {
		printk("proc_create failed\n");
		return -1;
	}

	printk("my module init success.\n");
	return 0;
}

static void __exit my_module_exit(void)
{
	if (proc_num) {
		proc_remove(proc_num);
	}
	
	printk("exit module.\n");
	return;
}

module_init(my_module_init);
module_exit(my_module_exit);


MODULE_AUTHOR("xdd");
MODULE_LICENSE("GPL");
