#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

//-------------------------------------------

static int __init kernel_panic_init(void)
{
    pr_emerg("kernelPanic: about to call panic()\n");

    panic("ldd_20261021_kernelPanic: intentional panic");

    return 0;
}

//-------------------------------------------

static void __exit kernel_panic_exit(void)
{
    pr_info("kernelPanic: exit\n");
}

//-------------------------------------------

module_init(kernel_panic_init);
module_exit(kernel_panic_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Intentional kernel panic demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


