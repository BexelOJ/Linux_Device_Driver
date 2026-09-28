#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>

//-------------------------------------------

static int __init device_tree_basic_init(void)
{
    struct device_node* root;
    struct device_node* child;

    root = of_find_node_by_path("/");

    if (!root) {
        pr_err("DT root not found\n");
        return -ENODEV;
    }

    pr_info("Device Tree root: %s\n",
        root->full_name);

    for_each_child_of_node(root, child) {

        pr_info("Child node: %s\n",
            child->full_name);
    }

    of_node_put(root);

    return 0;
}

//-------------------------------------------

static void __exit device_tree_basic_exit(void)
{
    pr_info("Device Tree basic example unloaded\n");
}

//-------------------------------------------

module_init(device_tree_basic_init);
module_exit(device_tree_basic_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic Device Tree traversal");



