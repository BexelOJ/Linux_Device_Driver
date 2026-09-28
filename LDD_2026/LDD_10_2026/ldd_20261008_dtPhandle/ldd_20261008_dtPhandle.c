#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>

//-------------------------------------------

static int __init dt_phandle_init(void)
{
    struct device_node* consumer;
    struct device_node* provider;

    consumer = of_find_node_by_name(NULL, "consumer");

    if (!consumer) {
        pr_err("consumer node not found\n");
        return -ENODEV;
    }

    provider = of_parse_phandle(consumer, "provider", 0);

    if (!provider) {
        pr_err("provider phandle not found\n");
        of_node_put(consumer);
        return -ENODEV;
    }

    pr_info("Consumer : %s\n", consumer->full_name);
    pr_info("Provider : %s\n", provider->full_name);

    of_node_put(provider);
    of_node_put(consumer);

    return 0;
}

//-------------------------------------------

static void __exit dt_phandle_exit(void)
{
    pr_info("DT phandle example unloaded\n");
}

//-------------------------------------------

module_init(dt_phandle_init);
module_exit(dt_phandle_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Tree phandle example");



