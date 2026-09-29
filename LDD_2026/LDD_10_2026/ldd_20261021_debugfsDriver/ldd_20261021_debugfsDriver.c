#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>

//-------------------------------------------

static struct dentry* debugfs_dir;
static struct dentry* debugfs_value;

static int debug_value = 100;

//-------------------------------------------

static const struct file_operations debugfs_fops =
{
    .owner = THIS_MODULE,
};

//-------------------------------------------

static int __init debugfs_driver_init(void)
{
    pr_info("debugfsDriver: module loaded\n");

    debugfs_dir = debugfs_create_dir("ldd_debugfs", NULL);

    if (!debugfs_dir)
        return -ENOMEM;

    debugfs_value =
        debugfs_create_u32("value",
            0644,
            debugfs_dir,
            (u32*)&debug_value);

    if (!debugfs_value)
    {
        debugfs_remove(debugfs_dir);
        return -ENOMEM;
    }

    return 0;
}

//-------------------------------------------

static void __exit debugfs_driver_exit(void)
{
    debugfs_remove_recursive(debugfs_dir);

    pr_info("debugfsDriver: module unloaded\n");
}

//-------------------------------------------

module_init(debugfs_driver_init);
module_exit(debugfs_driver_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux debugfs driver example");


