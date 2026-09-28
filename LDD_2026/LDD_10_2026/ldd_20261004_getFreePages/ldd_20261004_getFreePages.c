//-------------------------------------------
// ldd_20261004_getFreePages.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gfp.h>
#include <linux/mm.h>

#define ORDER 2

static unsigned long allocatedAddress;

//-------------------------------------------

static int __init get_free_pages_init(void)
{
    unsigned long size;

    printk(KERN_INFO
        "ldd_20261004_getFreePages: init\n");

    /*
     * order 2 means:
     *
     * 2^2 = 4 pages
     */

    size = (1UL << ORDER) * PAGE_SIZE;

    allocatedAddress = __get_free_pages(
        GFP_KERNEL,
        ORDER
    );

    if (!allocatedAddress)
    {
        printk(KERN_ERR
            "get_free_pages() failed\n");

        return -ENOMEM;
    }

    printk(KERN_INFO
        "Address : %px\n",
        (void*)allocatedAddress);

    printk(KERN_INFO
        "Order   : %d\n",
        ORDER);

    printk(KERN_INFO
        "Pages   : %lu\n",
        (1UL << ORDER));

    printk(KERN_INFO
        "Size    : %lu bytes\n",
        size);

    return 0;
}

//-------------------------------------------

static void __exit get_free_pages_exit(void)
{
    if (allocatedAddress)
    {
        free_pages(
            allocatedAddress,
            ORDER
        );

        printk(KERN_INFO
            "Pages freed\n");
    }

    printk(KERN_INFO
        "ldd_20261004_getFreePages: exit\n");
}

//-------------------------------------------

module_init(get_free_pages_init);
module_exit(get_free_pages_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("get_free_pages() demonstration");

//-------------------------------------------


