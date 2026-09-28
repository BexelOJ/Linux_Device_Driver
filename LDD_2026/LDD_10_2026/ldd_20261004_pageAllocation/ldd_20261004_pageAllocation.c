//-------------------------------------------
// ldd_20261004_kfree.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

#define BUFFER_SIZE 256

static char* buffer;

//-------------------------------------------

static int __init kfree_init(void)
{
    printk(KERN_INFO
        "ldd_20261004_kfree: init\n");

    buffer = kmalloc(
        BUFFER_SIZE,
        GFP_KERNEL
    );

    if (!buffer)
    {
        printk(KERN_ERR
            "kmalloc() failed\n");

        return -ENOMEM;
    }

    printk(KERN_INFO
        "Memory allocated: %px\n",
        buffer);

    memset(buffer, 0x55, BUFFER_SIZE);

    return 0;
}

//-------------------------------------------

static void __exit kfree_exit(void)
{
    if (buffer)
    {
        printk(KERN_INFO
            "Freeing: %px\n",
            buffer);

        kfree(buffer);

        buffer = NULL;

        printk(KERN_INFO
            "Memory released\n");
    }

    printk(KERN_INFO
        "ldd_20261004_kfree: exit\n");
}

//-------------------------------------------

module_init(kfree_init);
module_exit(kfree_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("kfree() demonstration");

//-------------------------------------------


