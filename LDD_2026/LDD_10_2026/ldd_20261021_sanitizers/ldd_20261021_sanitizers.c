#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

//-------------------------------------------

static int __init sanitizers_init(void)
{
    char* buffer;

    pr_info("sanitizers: module loaded\n");

    buffer = kmalloc(64, GFP_KERNEL);

    if (!buffer)
        return -ENOMEM;

    memset(buffer, 0, 64);

    pr_info("sanitizers: allocated 64 bytes\n");

    kfree(buffer);

    pr_info("sanitizers: memory released\n");

    return 0;
}

//-------------------------------------------

static void __exit sanitizers_exit(void)
{
    pr_info("sanitizers: module unloaded\n");
}

//-------------------------------------------

module_init(sanitizers_init);
module_exit(sanitizers_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux kernel sanitizers example");


