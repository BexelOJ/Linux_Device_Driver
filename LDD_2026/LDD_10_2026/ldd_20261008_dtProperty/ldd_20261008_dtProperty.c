#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>

//-------------------------------------------

static int __init dt_property_init(void)
{
    struct device_node* node;
    u32 reg_value;
    const char* device_name;

    node = of_find_node_by_name(NULL,
        "my_device");

    if (!node) {
        pr_err("my_device node not found\n");
        return -ENODEV;
    }

    if (!of_property_read_u32(node,
        "reg-value",
        &reg_value)) {

        pr_info("reg-value = 0x%x\n",
            reg_value);
    }

    if (!of_property_read_string(node,
        "device-name",
        &device_name)) {

        pr_info("device-name = %s\n",
            device_name);
    }

    of_node_put(node);

    return 0;
}

//-------------------------------------------

static void __exit dt_property_exit(void)
{
    pr_info("DT property example unloaded\n");
}

//-------------------------------------------

module_init(dt_property_init);
module_exit(dt_property_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Tree property example");


/*
//-------------------------------------------



//-------------------------------------------
*/


