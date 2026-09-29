#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>

//-------------------------------------------

static int __init dt_reg_init(void)
{
    struct device_node* node;

    node = of_find_node_by_path("/");

    if (!node) {
        pr_err("Device Tree root node not found\n");
        return -ENODEV;
    }

    pr_info("Device Tree root node found\n");
    pr_info("Node name: %s\n", node->name);
    pr_info("Full name: %s\n", node->full_name);

    of_node_put(node);

    return 0;
}

//-------------------------------------------

static void __exit dt_reg_exit(void)
{
    pr_info("DT registration example unloaded\n");
}

//-------------------------------------------

module_init(dt_reg_init);
module_exit(dt_reg_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Device Tree basic registration example");


/*
//-------------------------------------------



//-------------------------------------------
*/


