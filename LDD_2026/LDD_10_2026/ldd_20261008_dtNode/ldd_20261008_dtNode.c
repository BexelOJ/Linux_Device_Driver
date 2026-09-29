#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>

//-------------------------------------------

static int __init dt_node_init(void)
{
    struct device_node* node;

    node = of_find_node_by_name(NULL, "chosen");

    if (!node) {
        pr_err("'chosen' node not found\n");
        return -ENODEV;
    }

    pr_info("Node found\n");
    pr_info("name      : %s\n", node->name);
    pr_info("full_name : %s\n", node->full_name);

    of_node_put(node);

    return 0;
}

//-------------------------------------------

static void __exit dt_node_exit(void)
{
    pr_info("DT node example unloaded\n");
}

//-------------------------------------------

module_init(dt_node_init);
module_exit(dt_node_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Tree node lookup example");


/*
//-------------------------------------------



//-------------------------------------------
*/


