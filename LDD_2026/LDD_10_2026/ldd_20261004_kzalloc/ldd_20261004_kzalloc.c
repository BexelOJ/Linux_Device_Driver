//-------------------------------------------
// ldd_20261004_kzalloc.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

#define BUFFER_SIZE 1024

static char* buffer;

//-------------------------------------------

static int __init kzalloc_init(void)
{
    printk(KERN_INFO
        "ldd_20261004_kzalloc: init\n");

    buffer = kzalloc(
        BUFFER_SIZE,
        GFP_KERNEL
    );

    if (!buffer)
    {
        printk(KERN_ERR
            "kzalloc() failed\n");

        return -ENOMEM;
    }

    printk(KERN_INFO
        "Buffer allocated and zero initialized\n");

    printk(KERN_INFO
        "Address: %px\n",
        buffer);

    printk(KERN_INFO
        "First byte: %d\n",
        buffer[0]);

    return 0;
}

//-------------------------------------------

static void __exit kzalloc_exit(void)
{
    if (buffer)
    {
        kfree(buffer);

        printk(KERN_INFO
            "Buffer freed\n");
    }

    printk(KERN_INFO
        "ldd_20261004_kzalloc: exit\n");
}

//-------------------------------------------

module_init(kzalloc_init);
module_exit(kzalloc_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("kzalloc() demonstration");

//-------------------------------------------


