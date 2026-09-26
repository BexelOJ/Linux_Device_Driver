#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/sched.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_20261003_nonBlockingIO"

//---------------------------------------------------

static dev_t ldd_devNumber;

static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

//---------------------------------------------------

static DECLARE_WAIT_QUEUE_HEAD(
    ldd_waitQueue
);

static bool ldd_dataReady = false;

//---------------------------------------------------

static int ldd_open(
    struct inode* inode,
    struct file* file
)
{
    pr_info(
        "ldd_20261003_nonBlockingIO: "
        "Device opened\n"
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
        "ldd_20261003_nonBlockingIO: "
        "Device closed\n"
    );

    return 0;
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
        "Data received from kernel\n";

    size_t length =
        sizeof(message) - 1;

    //------------------------------------------------
    // No data available
    //------------------------------------------------

    if (!ldd_dataReady) {

        //------------------------------------------------
        // Non-blocking mode
        //------------------------------------------------

        if (file->f_flags & O_NONBLOCK) {

            pr_info(
                "ldd_20261003_nonBlockingIO: "
                "No data - returning EAGAIN\n"
            );

            return -EAGAIN;
        }

        //------------------------------------------------
        // Blocking mode
        //------------------------------------------------

        pr_info(
            "ldd_20261003_nonBlockingIO: "
            "Waiting for data\n"
        );

        if (wait_event_interruptible(
            ldd_waitQueue,
            ldd_dataReady)) {

            return -ERESTARTSYS;
        }
    }

    //------------------------------------------------
    // Data is available
    //------------------------------------------------

    if (count < length) {
        return -EINVAL;
    }

    if (copy_to_user(
        buffer,
        message,
        length)) {

        return -EFAULT;
    }

    ldd_dataReady = false;

    pr_info(
        "ldd_20261003_nonBlockingIO: "
        "Data read successfully\n"
    );

    return length;
}

//---------------------------------------------------
// Generate a test event
//---------------------------------------------------

static ssize_t ldd_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* position
)
{
    pr_info(
        "ldd_20261003_nonBlockingIO: "
        "Generating data event\n"
    );

    //------------------------------------------------
    // Mark data as available
    //------------------------------------------------

    ldd_dataReady = true;

    //------------------------------------------------
    // Wake processes waiting in read()
    //------------------------------------------------

    wake_up_interruptible(
        &ldd_waitQueue
    );

    return count;
}

//---------------------------------------------------

static const struct file_operations ldd_fops = {

    .owner = THIS_MODULE,

    .open = ldd_open,

    .release = ldd_release,

    .read = ldd_read,

    .write = ldd_write,
};

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    int ret;

    pr_info(
        "ldd_20261003_nonBlockingIO: "
        "Module initialized\n"
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
            "ldd_20261003_nonBlockingIO: "
            "alloc_chrdev_region failed\n"
        );

        return ret;
    }

    //------------------------------------------------
    // Initialize character device
    //------------------------------------------------

    cdev_init(
        &ldd_cdev,
        &ldd_fops
    );

    ldd_cdev.owner = THIS_MODULE;

    //------------------------------------------------
    // Add character device
    //------------------------------------------------

    ret = cdev_add(
        &ldd_cdev,
        ldd_devNumber,
        1
    );

    if (ret < 0) {

        pr_err(
            "ldd_20261003_nonBlockingIO: "
            "cdev_add failed\n"
        );

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return ret;
    }

    //------------------------------------------------
    // Create class
    //------------------------------------------------

    ldd_class = class_create(
        DEVICE_NAME
    );

    if (IS_ERR(ldd_class)) {

        cdev_del(
            &ldd_cdev
        );

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        return PTR_ERR(
            ldd_class
        );
    }

    //------------------------------------------------
    // Create device
    //------------------------------------------------

    ldd_device = device_create(
        ldd_class,
        NULL,
        ldd_devNumber,
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(ldd_device)) {

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

        return PTR_ERR(
            ldd_device
        );
    }

    //------------------------------------------------

    pr_info(
        "ldd_20261003_nonBlockingIO: "
        "Device created: /dev/%s\n",
        DEVICE_NAME
    );

    return 0;
}

//---------------------------------------------------

static void __exit ldd_moduleExit(void)
{
    //------------------------------------------------
    // Remove device
    //------------------------------------------------

    device_destroy(
        ldd_class,
        ldd_devNumber
    );

    //------------------------------------------------
    // Remove class
    //------------------------------------------------

    class_destroy(
        ldd_class
    );

    //------------------------------------------------
    // Remove cdev
    //------------------------------------------------

    cdev_del(
        &ldd_cdev
    );

    //------------------------------------------------
    // Release device number
    //------------------------------------------------

    unregister_chrdev_region(
        ldd_devNumber,
        1
    );

    pr_info(
        "ldd_20261003_nonBlockingIO: "
        "Module exited\n"
    );
}

//---------------------------------------------------

module_init(
    ldd_moduleInit
);

module_exit(
    ldd_moduleExit
);

//---------------------------------------------------

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Er Bexel O J");

MODULE_DESCRIPTION(
    "Linux kernel non-blocking I/O character driver"
);

//---------------------------------------------------



