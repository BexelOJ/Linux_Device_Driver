#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

//-------------------------------------------

static int ldd_refcount_open(struct inode* inode,
    struct file* file)
{
    pr_info("moduleRefcount: open()\n");

    /*
     * When a userspace process opens the device,
     * the module is considered to be in use.
     *
     * The kernel normally manages the module
     * reference count through the file_operations
     * owner = THIS_MODULE.
     */

    return 0;
}

//-------------------------------------------

static int ldd_refcount_release(struct inode* inode,
    struct file* file)
{
    pr_info("moduleRefcount: release()\n");

    return 0;
}

//-------------------------------------------

static const struct file_operations ldd_refcount_fops =
{
    .owner = THIS_MODULE,
    .open = ldd_refcount_open,
    .release = ldd_refcount_release,
};

//-------------------------------------------

static struct miscdevice ldd_refcount_device =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = "ldd_refcount",
    .fops = &ldd_refcount_fops,
};

//-------------------------------------------

static int __init module_refcount_init(void)
{
    int ret;

    pr_info("moduleRefcount: init()\n");

    ret = misc_register(&ldd_refcount_device);

    if (ret)
    {
        pr_err("moduleRefcount: misc_register failed\n");
        return ret;
    }

    pr_info("moduleRefcount: /dev/ldd_refcount created\n");

    return 0;
}

//-------------------------------------------

static void __exit module_refcount_exit(void)
{
    misc_deregister(&ldd_refcount_device);

    pr_info("moduleRefcount: exit()\n");
}

//-------------------------------------------

module_init(module_refcount_init);
module_exit(module_refcount_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux module reference counting example");


/*


insmod
  │
  ▼
Module loaded
  │
  ▼
/dev/ldd_refcount
  │
  │ open()
  ▼
Module in use
  │
  │ close()
  ▼
Module can become unloadable


*/


