#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_NAME "ldd_20261025_yoctoRecipe"

static int __init yoctoRecipe_init(void)
{
    pr_info("%s: Yocto recipe-built module loaded\n",
        DRIVER_NAME);

    return 0;
}

static void __exit yoctoRecipe_exit(void)
{
    pr_info("%s: Yocto recipe-built module unloaded\n",
        DRIVER_NAME);
}

module_init(yoctoRecipe_init);
module_exit(yoctoRecipe_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Yocto recipe kernel module example");


/*
//-------------------------------------------



//-------------------------------------------
*/


