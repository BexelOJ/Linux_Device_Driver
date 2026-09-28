//-------------------------------------------
// ldd_20261004_kmalloc.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

#define BUFFER_SIZE 1024

static char* buffer;

//-------------------------------------------

static int __init kmalloc_init(void)
{
    printk(KERN_INFO
        "ldd_20261004_kmalloc: init\n");

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

    memset(buffer, 0xAA, BUFFER_SIZE);

    printk(KERN_INFO
        "Buffer allocated\n");

    printk(KERN_INFO
        "Address: %px\n",
        buffer);

    printk(KERN_INFO
        "Size: %d bytes\n",
        BUFFER_SIZE);

    return 0;
}

//-------------------------------------------

static void __exit kmalloc_exit(void)
{
    if (buffer)
    {
        kfree(buffer);

        printk(KERN_INFO
            "Buffer freed using kfree()\n");
    }

    printk(KERN_INFO
        "ldd_20261004_kmalloc: exit\n");
}

//-------------------------------------------

module_init(kmalloc_init);
module_exit(kmalloc_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("kmalloc() demonstration");

//-------------------------------------------


