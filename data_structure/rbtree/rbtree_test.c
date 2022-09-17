#include <linux/init.h>
#include <linux/module.h>


/*学习红黑树的使用,首先插入10个数，然后从小到大输出它们*/

struct num_node {
	int num;
	struct rb_node rbtree_node;
};

#define INSERT_NUM 10

static rb_root num_root;

static 

static int __init rbtree_learn_init(void)
{
	struct num_node *node;
	int i;

	for (i = 0; i < INSERT_NUM; i++) {
		node = kmalloc(sizeof(struct num_node), GFP_KERNEL);
		if (!node) {
			pr_error("kmalloc failed\n");
			return -1;
		}

		node->num = i;
		insert_rbnode(node);
	}

	printk("init success\n");
	printf("start show nodes:\n");
	
	show_rbnode();	
}


static void __exit rbtree_learn_exit(void)
{

}


module_init(rbtree_learn_init);
module_exit(rbtree_learn_exit);


MODULE_AUTHOR("tomasphony");
MODULE_LICENSE("GPL");
