#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/slab.h>

/*每次打开seq_file_demo2文件都会修改node中的seq值*/


int node_num = 0;
static struct dentry *seq_file_demo_dir;
LIST_HEAD(seq_file_node_head);

struct seq_demo_node {
    char name[10];
    int seq;
    struct list_head list;
};

static int seq_demo_show(struct seq_file *file, void *v)
{
    struct seq_demo_node *node;
    
    if (!list_empty(&seq_file_node_head)) {
        list_for_each_entry(node, &seq_file_node_head, list) {
            seq_printf(file, "name:%s, node->seq:%d, address:0x%p\n", 
                                node->name, node->seq, node);
        }
    }

    if(!list_empty(&seq_file_node_head)) {
        list_for_each_entry(node, &seq_file_node_head, list) {
            node->seq = node_num;
            node_num++;
        }
    }

    return 0;
}

static int seq_demo2_open(struct inode *inode, struct file *file)
{
    return single_open(file, seq_demo_show, NULL);
}


static struct file_operations seq_file_demo_fops = {
        .owner = THIS_MODULE,
        .open = seq_demo2_open,
        .read = seq_read,
        .release = seq_release,
        .llseek = seq_lseek,
};

static int __init seq_file_demo2_init(void)
{
    int i;
    struct seq_demo_node *node;
    seq_file_demo_dir = debugfs_create_file("seq_file_demo2",  0444, NULL, NULL, &seq_file_demo_fops);
    if (!seq_file_demo_dir) {
        pr_err("debugfs_create_file failed\n");
        return -1;
    }

    for (i = 0; i < 10; i++) {
        node = kmalloc(sizeof(struct seq_demo_node), GFP_KERNEL);
        if (!node) {
            pr_err("kmalloc failed\n");
            return -1;
        }

        sprintf(node->name, "node%d", i);
        node->seq = node_num;
        node_num++;
        INIT_LIST_HEAD(&node->list);

        list_add_tail(&node->list, &seq_file_node_head);
    }

    return 0;
}


static void __exit seq_file_demo2_exit(void)
{
    struct seq_demo_node *pos, *next;
    
    if (seq_file_demo_dir) {
        debugfs_remove(seq_file_demo_dir);
        list_for_each_entry_safe(pos, next, &seq_file_node_head, list) {
            printk("release node:%s\n", pos->name);
            kfree(pos);
        }
    }

    pr_info("exit module success.\n");
}


module_init(seq_file_demo2_init);
module_exit(seq_file_demo2_exit);


