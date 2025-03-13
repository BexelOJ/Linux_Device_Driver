#include <linux/module.h>    // Required for all kernel modules
#include <linux/kernel.h>    // For KERN_INFO
#include <linux/init.h>      // For the macros
#include <linux/fs.h>        // Required for file operations

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple Hello World Linux Driver");

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello, World! Kernel Module Loaded.\n");
    return 0; // Return 0 to indicate successful loading
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye, World! Kernel Module Unloaded.\n");
}

module_init(hello_init);  // Specify the entry function
module_exit(hello_exit);  // Specify the exit function

