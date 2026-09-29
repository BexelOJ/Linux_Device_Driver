#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define PROC_NAME "ldd_20261003_procfs"

static struct proc_dir_entry* ldd_procEntry;

static char ldd_data[128] = "Hello from Linux procfs\n";

//-------------------------------------------
// procfs read
//-------------------------------------------

static ssize_t ldd_procRead(
    struct file* file,
    char __user* buffer,
    size_t count,
    loff_t* offset)
{
    size_t dataLength;

    pr_info("ldd_20261003_procfs: read called\n");

    dataLength = strlen(ldd_data);

    if (*offset >= dataLength)
        return 0;

    if (count > dataLength - *offset)
        count = dataLength - *offset;

    if (copy_to_user(buffer, ldd_data + *offset, count))
        return -EFAULT;

    *offset += count;

    return count;
}

//-------------------------------------------
// procfs write
//-------------------------------------------

static ssize_t ldd_procWrite(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    size_t copySize;

    pr_info("ldd_20261003_procfs: write called\n");

    copySize = min(count, sizeof(ldd_data) - 1);

    memset(ldd_data, 0, sizeof(ldd_data));

    if (copy_from_user(ldd_data, buffer, copySize))
        return -EFAULT;

    ldd_data[copySize] = '\0';

    pr_info(
        "ldd_20261003_procfs: data = %s\n",
        ldd_data
    );

    return copySize;
}

//-------------------------------------------
// procfs operations
//-------------------------------------------

static const struct proc_ops ldd_procOps =
{
    .proc_read = ldd_procRead,
    .proc_write = ldd_procWrite,
};

//-------------------------------------------
// Module initialization
//-------------------------------------------

static int __init ldd_init(void)
{
    pr_info("ldd_20261003_procfs: module loaded\n");

    ldd_procEntry = proc_create(
        PROC_NAME,
        0666,
        NULL,
        &ldd_procOps
    );

    if (!ldd_procEntry)
    {
        pr_err("ldd_20261003_procfs: proc_create failed\n");
        return -ENOMEM;
    }

    pr_info(
        "ldd_20261003_procfs: /proc/%s created\n",
        PROC_NAME
    );

    return 0;
}

//-------------------------------------------
// Module cleanup
//-------------------------------------------

static void __exit ldd_exit(void)
{
    proc_remove(ldd_procEntry);

    pr_info(
        "ldd_20261003_procfs: /proc/%s removed\n",
        PROC_NAME
    );

    pr_info("ldd_20261003_procfs: module unloaded\n");
}

//-------------------------------------------
// Module registration
//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------
// Module information
//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic Linux procfs driver example");


/*
//-------------------------------------------



//-------------------------------------------
*/


