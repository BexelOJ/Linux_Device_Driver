#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Exin Innvo Labs - Bexel");
MODULE_DESCRIPTION("Our First Dynamically Lodable Kernel Module");

static int ei_labs_module_init (void){
    printk("Hello, World!, from EI_Labs\n");
    return 0;
}

static void ei_labs_module_exit (void)
{
    printk("Good, Bye!, from EI_Labs\n");
}

module_init(ei_labs_module_init);
module_exit(ei_labs_module_exit);


