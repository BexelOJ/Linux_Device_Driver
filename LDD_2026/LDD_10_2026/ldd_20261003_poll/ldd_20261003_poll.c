#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include <linux/wait.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_20261003_poll"

//---------------------------------------------------

static dev_t ldd_devNumber;

static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

//---------------------------------------------------

static DECLARE_WAIT_QUEUE_HEAD(
    ldd_waitQueue
);

static bool ldd_dataReady;

//---------------------------------------------------

static int ldd_open(
    struct inode* inode,
    struct file* file
)
{
    pr_info(
        "ldd_20261003_poll: Device opened\n"
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
        "ldd_20261003_poll: Device closed\n"
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
        "Data from kernel driver\n";

    size_t length =
        sizeof(message) - 1;

    //------------------------------------------------
    // No data available
    //------------------------------------------------

    if (!ldd_dataReady) {

        if (file->f_flags & O_NONBLOCK) {

            pr_info(
                "ldd_20261003_poll: "
                "No data - EAGAIN\n"
            );

            return -EAGAIN;
        }

        //------------------------------------------------
        // Blocking read
        //------------------------------------------------

        if (wait_event_interruptible(
            ldd_waitQueue,
            ldd_dataReady)) {

            return -ERESTARTSYS;
        }
    }

    //------------------------------------------------
    // Check user buffer size
    //------------------------------------------------

    if (count < length) {
        return -EINVAL;
    }

    //------------------------------------------------
    // Copy data to user
    //------------------------------------------------

    if (copy_to_user(
        buffer,
        message,
        length)) {

        return -EFAULT;
    }

    //------------------------------------------------
    // Data consumed
    //------------------------------------------------

    ldd_dataReady = false;

    pr_info(
        "ldd_20261003_poll: "
        "Data read\n"
    );

    return length;
}

//---------------------------------------------------
// Write generates a test event
//---------------------------------------------------

static ssize_t ldd_write(
    struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* position
)
{
    pr_info(
        "ldd_20261003_poll: "
        "Event generated\n"
    );

    //------------------------------------------------
    // Mark data as available
    //------------------------------------------------

    ldd_dataReady = true;

    //------------------------------------------------
    // Wake processes waiting in poll/read
    //------------------------------------------------

    wake_up_interruptible(
        &ldd_waitQueue
    );

    return count;
}

//---------------------------------------------------
// poll() callback
//---------------------------------------------------

static __poll_t ldd_poll(
    struct file* file,
    poll_table* wait
)
{
    __poll_t mask = 0;

    pr_info(
        "ldd_20261003_poll: "
        "poll callback called\n"
    );

    //------------------------------------------------
    // Add current process to wait queue
    //------------------------------------------------

    poll_wait(
        file,
        &ldd_waitQueue,
        wait
    );

    //------------------------------------------------
    // Report device state
    //------------------------------------------------

    if (ldd_dataReady) {

        mask |= EPOLLIN;
        mask |= EPOLLRDNORM;
    }

    return mask;
}

//---------------------------------------------------

static const struct file_operations ldd_fops = {

    .owner = THIS_MODULE,

    .open = ldd_open,

    .release = ldd_release,

    .read = ldd_read,

    .write = ldd_write,

    .poll = ldd_poll,
};

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    int ret;

    pr_info(
        "ldd_20261003_poll: "
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
            "ldd_20261003_poll: "
            "alloc_chrdev_region failed\n"
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

    //------------------------------------------------
    // Add cdev
    //------------------------------------------------

    ret = cdev_add(
        &ldd_cdev,
        ldd_devNumber,
        1
    );

    if (ret < 0) {

        pr_err(
            "ldd_20261003_poll: "
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
        "ldd_20261003_poll: "
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
        "ldd_20261003_poll: "
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
    "Linux kernel poll character driver"
);

//---------------------------------------------------



