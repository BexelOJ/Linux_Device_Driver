#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>

//-------------------------------------------

static int __init dt_compatible_init(void)
{
    struct device_node* node;
    const char* compatible;

    node = of_find_node_by_name(NULL, "chosen");

    if (!node) {
        pr_err("Node not found\n");
        return -ENODEV;
    }

    if (!of_property_read_string(node,
        "compatible",
        &compatible)) {

        pr_info("compatible = %s\n",
            compatible);
    }
    else {
        pr_info("No compatible property found\n");
    }

    of_node_put(node);

    return 0;
}

//-------------------------------------------

static void __exit dt_compatible_exit(void)
{
    pr_info("DT compatible example unloaded\n");
}

//-------------------------------------------

module_init(dt_compatible_init);
module_exit(dt_compatible_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Tree compatible property example");



