#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "character_sensor"

static dev_t deviceNumber;
static struct cdev sensorCdev;
static struct class* sensorClass;

static int sensorValue = 2500;

//-------------------------------------------
// Read sensor value
//-------------------------------------------

static ssize_t sensor_read(struct file* file,
    char __user* buffer,
    size_t count,
    loff_t* offset)
{
    char data[32];
    int length;

    length = scnprintf(data, sizeof(data),
        "Sensor: %d mV\n", sensorValue);

    if (*offset >= length)
        return 0;

    if (copy_to_user(buffer, data, length))
        return -EFAULT;

    *offset += length;

    return length;
}

//-------------------------------------------
// Write sensor value
//-------------------------------------------

static ssize_t sensor_write(struct file* file,
    const char __user* buffer,
    size_t count,
    loff_t* offset)
{
    int value;

    if (kstrtoint_from_user(buffer, count, 10, &value))
        return -EINVAL;

    sensorValue = value;

    pr_info("character_sensor: value = %d\n", sensorValue);

    return count;
}

//-------------------------------------------
// File operations
//-------------------------------------------

static const struct file_operations sensorFops = {
    .owner = THIS_MODULE,
    .read = sensor_read,
    .write = sensor_write,
};

//-------------------------------------------
// Module initialization
//-------------------------------------------

static int __init character_sensor_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&deviceNumber, 0, 1, DEVICE_NAME);
    if (ret)
        return ret;

    cdev_init(&sensorCdev, &sensorFops);

    ret = cdev_add(&sensorCdev, deviceNumber, 1);
    if (ret)
        goto unregister_device;

    sensorClass = class_create(DEVICE_NAME);
    if (IS_ERR(sensorClass)) {
        ret = PTR_ERR(sensorClass);
        goto delete_cdev;
    }

    if (IS_ERR(device_create(sensorClass,
        NULL,
        deviceNumber,
        NULL,
        DEVICE_NAME))) {
        ret = -EINVAL;
        goto destroy_class;
    }

    pr_info("character_sensor: loaded\n");

    return 0;

destroy_class:
    class_destroy(sensorClass);

delete_cdev:
    cdev_del(&sensorCdev);

unregister_device:
    unregister_chrdev_region(deviceNumber, 1);

    return ret;
}

//-------------------------------------------
// Module cleanup
//-------------------------------------------

static void __exit character_sensor_exit(void)
{
    device_destroy(sensorClass, deviceNumber);
    class_destroy(sensorClass);
    cdev_del(&sensorCdev);
    unregister_chrdev_region(deviceNumber, 1);

    pr_info("character_sensor: unloaded\n");
}

module_init(character_sensor_init);
module_exit(character_sensor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Character device sensor driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


