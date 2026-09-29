#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

//-------------------------------------------

static void* buffer_a;
static void* buffer_b;

//-------------------------------------------

static int __init cleanup_init(void)
{
    int ret = 0;

    pr_info("cleanup: init()\n");

    buffer_a = kmalloc(1024, GFP_KERNEL);

    if (!buffer_a)
    {
        ret = -ENOMEM;
        goto error_a;
    }

    pr_info("cleanup: buffer_a allocated\n");

    buffer_b = kmalloc(2048, GFP_KERNEL);

    if (!buffer_b)
    {
        ret = -ENOMEM;
        goto error_b;
    }

    pr_info("cleanup: buffer_b allocated\n");

    return 0;

    //-------------------------------------------

error_b:

    kfree(buffer_a);
    buffer_a = NULL;

error_a:

    pr_err("cleanup: initialization failed\n");

    return ret;
}

//-------------------------------------------

static void __exit cleanup_exit(void)
{
    kfree(buffer_b);
    buffer_b = NULL;

    kfree(buffer_a);
    buffer_a = NULL;

    pr_info("cleanup: module unloaded\n");
}

//-------------------------------------------

module_init(cleanup_init);
module_exit(cleanup_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux kernel cleanup pattern example");



