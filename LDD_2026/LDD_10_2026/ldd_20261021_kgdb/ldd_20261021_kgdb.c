#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

//-------------------------------------------

static int kgdb_counter;

//-------------------------------------------

static void kgdb_debug_function(void)
{
    int i;

    for (i = 0; i < 5; i++)
    {
        kgdb_counter++;

        pr_info("kgdb: counter = %d\n",
            kgdb_counter);
    }
}

//-------------------------------------------

static int __init kgdb_init(void)
{
    pr_info("kgdb: module loaded\n");

    kgdb_debug_function();

    return 0;
}

//-------------------------------------------

static void __exit kgdb_exit(void)
{
    pr_info("kgdb: module unloaded\n");
}

//-------------------------------------------

module_init(kgdb_init);
module_exit(kgdb_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("KGDB learning example");


/*
//-------------------------------------------



//-------------------------------------------
*/


