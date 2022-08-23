#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>



/*学习内核线程的使用方法*/

static void show_info(void)
{
	struct task_struct *task = current;
	printk("pid:%d, nice:%d, prio:%d, static_prio:%d, comm:%s\n",
			task->pid, PRIO_TO_NICE(task->static_prio), task->prio,
			task->static_prio, task->comm);
}


static int kthread_fun(void *data)
{
	int i;
	/*这里传参还有问题，当i为0时参数不正确*/
	if (data == NULL) {
		i = 0;
	} else { 
		i = *((int *)data);
	}
	
	printk("%s: kthread_test%d is creating....\n", __func__, i);
	do {
		printk("kthread_test%d sleep in thread function.\n", i);
		msleep_interruptible(2000);
		printk("kthread_test%d sleep over.", i);
		show_info();
	} while (!kthread_should_stop());

	return 0;
}

static struct task_struct *tsk[NR_CPUS];

static int __init my_module_init(void)
{
	int i;
	printk("my module init\n");

	for_each_online_cpu(i) {
		//将cpu id作为形参传递到线程执行函数
		printk("kthread_test%d is will to create.\n", i);
		tsk[i] = kthread_run(kthread_fun, &i, "kthread_test%d", i);
		if (!tsk[i]) {
			printk("Failed to create a kernel thread\n");
			return -1;
		}
	}

	printk("my module init success.\n");

	return 0;
}

static void __exit my_module_exit(void)
{
	int i;
	for_each_online_cpu(i) {
		printk("stop kthread %d\n", i);
		kthread_stop(tsk[i]);
		printk("kthread %d is stoped\n", i);
	}
	
}


module_init(my_module_init);
module_exit(my_module_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("xddd");

