#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "ldd_20261023_debugfsAdvanced"

static struct dentry* debugfs_dir;
static int debug_value = 100;

static int __init debugfsAdvanced_init(void)
{
    debugfs_dir = debugfs_create_dir(DRIVER_NAME, NULL);

    if (!debugfs_dir)
        return -ENOMEM;

    debugfs_create_u32("debug_value", 0644,
        debugfs_dir, &debug_value);

    pr_info("%s: debugfs directory created\n", DRIVER_NAME);

    return 0;
}

static void __exit debugfsAdvanced_exit(void)
{
    debugfs_remove_recursive(debugfs_dir);

    pr_info("%s: debugfs directory removed\n", DRIVER_NAME);
}

module_init(debugfsAdvanced_init);
module_exit(debugfsAdvanced_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Advanced debugfs example");


/*
//-------------------------------------------



//-------------------------------------------
*/


