//-------------------------------------------
// ldd_20261004_userMemory.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/uaccess.h>

static char kernelBuffer[100];

//-------------------------------------------
// Module Init
//-------------------------------------------

static int __init user_memory_init(void)
{
    const char __user* userBuffer = NULL;

    printk(KERN_INFO
        "ldd_20261004_userMemory: init\n");

    /*
     * Normally userBuffer would come from
     * a system call such as read/write.
     *
     * copy_from_user():
     *
     * User Space
     *     |
     *     v
     * Kernel Space
     */

    printk(KERN_INFO
        "copy_from_user() is used to safely copy data\n"
        "from user space to kernel space\n");

    printk(KERN_INFO
        "copy_to_user() is used to safely copy data\n"
        "from kernel space to user space\n");

    return 0;
}

//-------------------------------------------
// Module Exit
//-------------------------------------------

static void __exit user_memory_exit(void)
{
    printk(KERN_INFO
        "ldd_20261004_userMemory: exit\n");
}

//-------------------------------------------

module_init(user_memory_init);
module_exit(user_memory_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("User and kernel memory demonstration");

//-------------------------------------------


