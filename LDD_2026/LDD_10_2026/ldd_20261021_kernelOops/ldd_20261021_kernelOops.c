#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

//-------------------------------------------

static int __init kernel_oops_init(void)
{
    int* ptr = NULL;

    pr_err("kernelOops: intentionally causing NULL dereference\n");

    /*
     * Intentional kernel fault.
     *
     * DO NOT USE ON A PRODUCTION SYSTEM.
     */

    *ptr = 1234;

    return 0;
}

//-------------------------------------------

static void __exit kernel_oops_exit(void)
{
    pr_info("kernelOops: exit\n");
}

//-------------------------------------------

module_init(kernel_oops_init);
module_exit(kernel_oops_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Intentional kernel Oops demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


