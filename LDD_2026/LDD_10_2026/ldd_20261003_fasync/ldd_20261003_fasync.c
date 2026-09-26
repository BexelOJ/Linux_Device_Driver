#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_20261003_epoll"

//---------------------------------------------------

static dev_t ldd_devNumber;
static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

static DECLARE_WAIT_QUEUE_HEAD(ldd_waitQueue);

static bool ldd_dataReady;

//---------------------------------------------------

static int ldd_open(
    struct inode* inode,
    struct file* file
)
{
    pr_info(
        "ldd_20261003_epoll: Device opened\n"
    );

    return 0;
}

//---------------------------------------------------

static int ldd_release(
    struct inode* inode,
    struct file* file
)
{
    pr_info(
        "ldd_20261003_epoll: Device closed\n"
    );

    return 0;
}

//---------------------------------------------------

static __poll_t ldd_poll(
    struct file* file,
    poll_table* wait
)
{
    __poll_t mask = 0;

    pr_info(
        "ldd_20261003_epoll: poll() called\n"
    );

    poll_wait(
        file,
        &ldd_waitQueue,
        wait
    );

    if (ldd_dataReady) {
        mask |= EPOLLIN | EPOLLRDNORM;
    }

    return mask;
}

//---------------------------------------------------

static ssize_t ldd_read(
    struct file* file,
    char __user* buffer,
    size_t count,
    loff_t* position
)
{
    char message[] =
        "Data from kernel driver\n";

    size_t length =
        sizeof(message);

    if (*position >= length) {
        return 0;
    }

    if (copy_to_user(
        buffer,
        message,
        length)) {

        return -EFAULT;
    }

    *position += length;

    ldd_dataReady = false;

    pr_info(
        "ldd_20261003_epoll: Data read\n"
    );

    return length;
}

//---------------------------------------------------

static const struct file_operations ldd_fops = {
    .owner = THIS_MODULE,
    .open = ldd_open,
    .release = ldd_release,
    .poll = ldd_poll,
    .read = ldd_read,
};

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    int ret;

    pr_info(
        "ldd_20261003_epoll: Module initialized\n"
    );

    //------------------------------------------------
    // Allocate device number
    //------------------------------------------------

    ret = alloc_chrdev_region(
        &ldd_devNumber,
        0,
        1,
        DEVICE_NAME
    );

    if (ret < 0) {
        pr_err(
            "ldd_20261003_epoll: alloc_chrdev_region failed\n"
        );

        return ret;
    }

    //------------------------------------------------
    // Initialize cdev
    //------------------------------------------------

    cdev_init(
        &ldd_cdev,
        &ldd_fops
    );

    ldd_cdev.owner = THIS_MODULE;

    ret = cdev_add(
        &ldd_cdev,
        ldd_devNumber,
        1
    );

    if (ret < 0) {
        pr_err(
            "ldd_20261003_epoll: cdev_add failed\n"
        );

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return ret;
    }

    //------------------------------------------------
    // Create device class
    //------------------------------------------------

    ldd_class = class_create(
        DEVICE_NAME
    );

    if (IS_ERR(ldd_class)) {

        cdev_del(&ldd_cdev);

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return PTR_ERR(ldd_class);
    }

    //------------------------------------------------
    // Create /dev device
    //------------------------------------------------

    ldd_device = device_create(
        ldd_class,
        NULL,
        ldd_devNumber,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(ldd_device)) {

        class_destroy(ldd_class);

        cdev_del(&ldd_cdev);

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return PTR_ERR(ldd_device);
    }

    //------------------------------------------------

    ldd_dataReady = true;

    wake_up_interruptible(
        &ldd_waitQueue
    );

    //------------------------------------------------

    pr_info(
        "ldd_20261003_epoll: Device created\n"
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    device_destroy(
        ldd_class,
        ldd_devNumber
    );

    class_destroy(
        ldd_class
    );

    cdev_del(
        &ldd_cdev
    );

    unregister_chrdev_region(
        ldd_devNumber,
        1
    );

    pr_info(
        "ldd_20261003_epoll: Module exited\n"
    );
}

//---------------------------------------------------

module_init(ldd_moduleInit);
module_exit(ldd_moduleExit);

//---------------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION(
    "Linux kernel epoll/poll integration example"
);

//---------------------------------------------------



