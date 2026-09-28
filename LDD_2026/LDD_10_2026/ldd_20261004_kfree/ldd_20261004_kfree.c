//-------------------------------------------
// ldd_20261004_memoryDebug.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

#define BUFFER_SIZE 64

static char* buffer;

//-------------------------------------------

static int __init memory_debug_init(void)
{
    printk(KERN_INFO
        "ldd_20261004_memoryDebug: init\n");

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

    memset(buffer, 0xAB, BUFFER_SIZE);

    printk(KERN_INFO
        "Allocated memory: %px\n",
        buffer);

    printk(KERN_INFO
        "Use tools such as SLUB debug, KASAN,\n"
        "kmemleak and CONFIG_DEBUG_KMEMLEAK\n"
        "to investigate kernel memory problems.\n");

    return 0;
}

//-------------------------------------------

static void __exit memory_debug_exit(void)
{
    if (buffer)
    {
        kfree(buffer);

        buffer = NULL;

        printk(KERN_INFO
            "Memory released\n");
    }

    printk(KERN_INFO
        "ldd_20261004_memoryDebug: exit\n");
}

//-------------------------------------------

module_init(memory_debug_init);
module_exit(memory_debug_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Kernel memory debugging demonstration");

//-------------------------------------------


