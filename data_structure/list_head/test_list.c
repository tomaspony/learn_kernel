#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/slab.h>

/*
	熟悉链表的使用：这个demo首先添加10个数字，
	将它们串为一个链表，之后通过遍历和删除来
	达到联系效果
*/

struct node_struct {
	int data;
	struct list_head list;
};

static int __init test_list_init(void)
{
	int i;
	struct node_struct *node;
	struct list_head *pos;
	LIST_HEAD(node_head);

	for (i = 0; i < 10; i++) {
		struct node_struct *node = 
				kmalloc(sizeof(struct node_struct), GFP_KERNEL);
		if (node == NULL) {
			printk("kmalloc failed\n");
			return -ENOMEM;
		}

		node->data = i;
		list_add_tail(&node->list, &node_head);
	}


	/*遍历链表*/
	list_for_each(pos, &node_head) {
		node = list_entry(pos, struct node_struct, list); 
		printk("node->data=%d\n", node->data);
	}

	/*删除元素*/
	list_for_each(pos, &node_head) {
		node = list_entry(pos, struct node_struct, list);
		if (node && node->data == 5) {
			printk("delete node node->data=%d\n", node->data);
			list_del(&node->list);
			kfree(node);
		}
	}

	/*继续遍历*/
	list_for_each_entry(node, &node_head, list) {
		printk("node->data = %d\n", node->data);
	}

	//list_for_each_entry_safe(pos, n, head, member)
	return 0;
}

static void __exit test_list_exit(void)
{
	printk("exit module.\n");
	return;
}


module_init(test_list_init);
module_exit(test_list_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("tomas");

