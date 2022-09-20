#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/rbtree.h>

#define MAX_RBNODE_NUM 10

struct my_node {
    int num;
    struct rb_node rb;
};

struct rb_root node_root = RB_ROOT;

static void insert_rbnode(struct my_node *my_data)
{
    struct rb_node **node, *parent = NULL;

    //ËÑË÷½áµã
    node = &(node_root.rb_node);
    while (*node) {
        struct my_node *tmp;
        parent = *node;

        tmp = container_of(*node, struct my_node, rb);
        if (tmp->num > my_data->num) {
            node = &((*node)->rb_left);
        } else if (tmp->num < my_data->num) {
            node = &((*node)->rb_right);
        } else {
            return ;
        }
    }

    printk("insert node\n");
    rb_link_node(&my_data->rb, parent, node);
    rb_insert_color(&my_data->rb, &node_root);
}

static int __init my_rbtree_test_init(void)
{
    int i;
    struct my_node *node;
    
    for (i = 0; i < MAX_RBNODE_NUM; i++) {
        node = kmalloc(sizeof(struct my_node), GFP_KERNEL);
        if (!node) {
            pr_err("kmalloc failed\n");
            return -1;
        }

        node->num = i;
        insert_rbnode(node);
    }

    printk("init module success");
    return 0;
}

static void __exit my_rbtree_test_exit(void)
{
    struct my_node *my_data;
    struct rb_node *node;

    for (node = rb_first(&node_root); node; node= rb_next(node)) {
        my_data = container_of(node, struct my_node, rb);
        printk("my_data->num:%d\n", my_data->num);
    }

    node = rb_first(&node_root);

    while (node) {
        rb_erase(node, &node_root);
        
        my_data = container_of(node, struct my_node, rb);
        if (my_data) {
            printk("release node node->num:%d\n", my_data->num);
            kfree(my_data);
        }

        node = rb_first(&node_root);
    }

    printk("exit modoule success\n");
}


module_init(my_rbtree_test_init);
module_exit(my_rbtree_test_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("tomasphony");
