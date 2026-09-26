#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>

//---------------------------------------------------

static struct dentry* ldd_debugfsDir;
static int ldd_value = 100;

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    pr_info(
        "ldd_20261003_debugfs: Module initialized\n"
    );

    ldd_debugfsDir = debugfs_create_dir(
        "ldd_20261003_debugfs",
        NULL
    );

    if (!ldd_debugfsDir) {
        pr_err(
            "ldd_20261003_debugfs: Failed to create debugfs directory\n"
        );

        return -ENOMEM;
    }

    debugfs_create_u32(
        "value",
        0644,
        ldd_debugfsDir,
        (u32*)&ldd_value
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    debugfs_remove_recursive(ldd_debugfsDir);

    pr_info(
        "ldd_20261003_debugfs: Module exited\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux kernel DebugFS example");

//---------------------------------------------------



