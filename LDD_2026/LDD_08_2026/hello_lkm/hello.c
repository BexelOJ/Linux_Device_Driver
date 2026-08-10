#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello_lkm: Module Loaded\n");
    pr_info("hello_lkm: module loaded\n");

    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "hello_lkm: Module Unloaded\n");
    pr_info("hello_lkm: module unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ExinInnvoLabs");
MODULE_DESCRIPTION("A sinple Linux Loadable Kernel Module");



