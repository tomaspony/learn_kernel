#include <linux/init.h>#include <linux/cdev.h>#include <linux/kernel.h>#include <linux/slab.h>#include <linux/module.h>#include <linux/fs.h>#include <linux/uaccess.h>

#define SECOND_MAJOR 232
static int second_major = SECOND_MAJOR;

struct second_dev {
	struct cdev cdev;
	atomic_t counter;
	struct timer_list s_timer;
};


static struct second_dev *second_devp;

static void second_timer_handler(struct timer_list *tm)
{
	atomic_inc(&second_devp->counter);
	mod_timer(tm, jiffies + HZ);
	printk("current jiffies is %ld\n", jiffies);
}




int second_open(struct inode *inode, struct file *filp)
{
	filp->private_data = second_devp;

	//timer_setup(timer, callback, 0)
	second_devp->s_timer.function = second_timer_handler;
	second_devp->s_timer.expires = jiffies + HZ;

	add_timer(&second_devp->s_timer);

	atomic_set(&second_devp->counter, 0);

	return 0;
}


static int second_release(struct inode *inode, struct file *filp)
{
	struct second_dev *dev = filp->private_data;

	del_timer(&dev->s_timer);

	return 0;
}

static ssize_t second_read(struct file *filp, char __user *buf, 
									size_t size, loff_t *ppos)
{
	struct second_dev *dev = filp->private_data;
	int count;
	int ret;
	
	count = atomic_read(&dev->counter);
	ret = copy_to_user(buf, &count, sizeof(int));
	if (ret) {
		return -EFAULT;
	}

	return sizeof(int);
}

static struct file_operations second_fops = {
	.owner = THIS_MODULE,
	.open = second_open,
	.release = second_release,
	.read = second_read,
};

static void second_cdev_setup(struct second_dev *sd)
{
	int err;
	cdev_init(&sd->cdev, &second_fops);
	err = cdev_add(&sd->cdev, MKDEV(second_major, 0), 1);
	if (err) {
		printk("Failed to add second device\n");
	}
}



static int __init second_init(void) 
{
	dev_t dev_no;
	int ret;
	dev_no = MKDEV(SECOND_MAJOR, 0);

	ret = register_chrdev_region(dev_no, 1, "second");
	if (ret < 0) {
		printk("register device failed, ret = %d\n", ret);
		return -1;
	}

	second_devp = kzalloc(sizeof(struct second_dev), GFP_KERNEL);
	if (second_devp == NULL) {
		printk("kzalloc failed\n");
		ret = -ENOMEM;
		goto unregister;
	}

	second_cdev_setup(second_devp);
	printk("second device init successed\n");
	return 0;

unregister:
	unregister_chrdev_region(dev_no, 1);
	return ret;
}

static void __exit second_exit(void)
{	//向系统删除一个cdev	cdev_del(&second_devp->cdev);	if (second_devp) 		kfree(second_devp);		unregister_chrdev_region(MKDEV(second_major, 0), 1);	printk("second device has been deleted\n");	return ;
}


module_init(second_init);
module_exit(second_exit);



MODULE_AUTHOR("tomaspony");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("learn how to use timer_list");


