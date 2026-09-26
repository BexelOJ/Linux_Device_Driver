#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

//---------------------------------------------------

#define DEVICE_NAME "ldd_20261003_mmap"

//---------------------------------------------------

static dev_t ldd_devNumber;

static struct cdev ldd_cdev;
static struct class* ldd_class;
static struct device* ldd_device;

static void* ldd_buffer;

//---------------------------------------------------

static int ldd_open(
    struct inode* inode,
    struct file* file
)
{
    pr_info(
        "ldd_20261003_mmap: Device opened\n"
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
        "ldd_20261003_mmap: Device closed\n"
    );

    return 0;
}

//---------------------------------------------------

static int ldd_mmap(
    struct file* file,
    struct vm_area_struct* vma
)
{
    unsigned long size;
    unsigned long pfn;

    //------------------------------------------------
    // Size requested by user
    //------------------------------------------------

    size = vma->vm_end - vma->vm_start;

    //------------------------------------------------
    // We allocated only one page
    //------------------------------------------------

    if (size > PAGE_SIZE) {

        pr_err(
            "ldd_20261003_mmap: "
            "Mapping size too large\n"
        );

        return -EINVAL;
    }

    //------------------------------------------------
    // Convert kernel virtual address to PFN
    //------------------------------------------------

    pfn = virt_to_pfn(
        ldd_buffer
    );

    //------------------------------------------------
    // Map physical page into user space
    //------------------------------------------------

    if (remap_pfn_range(
        vma,
        vma->vm_start,
        pfn,
        PAGE_SIZE,
        vma->vm_page_prot)) {

        pr_err(
            "ldd_20261003_mmap: "
            "remap_pfn_range failed\n"
        );

        return -EAGAIN;
    }

    pr_info(
        "ldd_20261003_mmap: "
        "Memory mapped to user space\n"
    );

    return 0;
}

//---------------------------------------------------

static const struct file_operations ldd_fops = {

    .owner = THIS_MODULE,

    .open = ldd_open,

    .release = ldd_release,

    .mmap = ldd_mmap,
};

//---------------------------------------------------

static int __init ldd_moduleInit(void)
{
    int ret;

    pr_info(
        "ldd_20261003_mmap: Module initialized\n"
    );

    //------------------------------------------------
    // Allocate one page
    //------------------------------------------------

    ldd_buffer = kmalloc(
        PAGE_SIZE,
        GFP_KERNEL
    );

    if (!ldd_buffer) {

        pr_err(
            "ldd_20261003_mmap: "
            "Failed to allocate memory\n"
        );

        return -ENOMEM;
    }

    //------------------------------------------------
    // Put test data into kernel memory
    //------------------------------------------------

    snprintf(
        ldd_buffer,
        PAGE_SIZE,
        "Hello from kernel mmap!\n"
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
            "ldd_20261003_mmap: "
            "alloc_chrdev_region failed\n"
        );

        kfree(ldd_buffer);

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
            "ldd_20261003_mmap: "
            "cdev_add failed\n"
        );

        unregister_chrdev_region(
            ldd_devNumber,
            1
        );

        kfree(ldd_buffer);

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

        kfree(ldd_buffer);

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

        kfree(ldd_buffer);

        return PTR_ERR(
            ldd_device
        );
    }

    //------------------------------------------------

    pr_info(
        "ldd_20261003_mmap: "
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

    //------------------------------------------------
    // Free kernel memory
    //------------------------------------------------

    kfree(
        ldd_buffer
    );

    pr_info(
        "ldd_20261003_mmap: Module exited\n"
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
    "Linux kernel mmap character driver"
);

//---------------------------------------------------



