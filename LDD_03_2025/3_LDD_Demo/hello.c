#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_init(void)
{
    printk(KERN_INFO "New Module Added\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Hello Module Exit\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Exin Innvo Labs");
MODULE_DESCRIPTION("Hello_World_Demo");

