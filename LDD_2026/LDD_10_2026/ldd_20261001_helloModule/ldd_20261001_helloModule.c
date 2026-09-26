#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//---------------------------------------------------

static int __init helloModule_init(void)
{
    pr_info("Hello from Linux Kernel Module\n");

    return 0;
}

//---------------------------------------------------

static void __exit helloModule_exit(void)
{
    pr_info("Goodbye from Linux Kernel Module\n");
}

//---------------------------------------------------

module_init(helloModule_init);
module_exit(helloModule_exit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("First Linux Device Driver learning module");

//---------------------------------------------------







