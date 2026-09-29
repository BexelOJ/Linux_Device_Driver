#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>

//-------------------------------------------

static int __init of_api_init(void)
{
    struct device_node* node;
    const char* value;
    u32 number;

    node = of_find_node_by_path("/");

    if (!node) {
        pr_err("Root node not found\n");
        return -ENODEV;
    }

    pr_info("Node: %s\n",
        node->full_name);

    if (!of_property_read_string(node,
        "model",
        &value)) {

        pr_info("model = %s\n", value);
    }

    if (!of_property_read_u32(node,
        "#address-cells",
        &number)) {

        pr_info("#address-cells = %u\n",
            number);
    }

    of_node_put(node);

    return 0;
}

//-------------------------------------------

static void __exit of_api_exit(void)
{
    pr_info("OF API example unloaded\n");
}

//-------------------------------------------

module_init(of_api_init);
module_exit(of_api_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Open Firmware Device Tree API example");


/*
//-------------------------------------------



//-------------------------------------------
*/


