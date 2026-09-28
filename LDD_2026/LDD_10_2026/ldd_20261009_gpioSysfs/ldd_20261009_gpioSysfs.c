#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/device.h>

//-------------------------------------------

static struct gpio_desc* gpio;

//-------------------------------------------

static ssize_t value_show(struct device* dev,
    struct device_attribute* attr,
    char* buf)
{
    int value;

    value = gpiod_get_value_cansleep(gpio);

    return sysfs_emit(buf, "%d\n", value);
}

//-------------------------------------------

static ssize_t value_store(struct device* dev,
    struct device_attribute* attr,
    const char* buf,
    size_t count)
{
    int value;
    int ret;

    ret = kstrtoint(buf, 10, &value);

    if (ret)
        return ret;

    gpiod_set_value_cansleep(gpio, value);

    return count;
}

//-------------------------------------------

static DEVICE_ATTR_RW(value);

//-------------------------------------------

static int gpio_sysfs_probe(struct platform_device* pdev)
{
    int ret;

    gpio = devm_gpiod_get(&pdev->dev,
        "control",
        GPIOD_OUT_LOW);

    if (IS_ERR(gpio))
        return PTR_ERR(gpio);

    ret = device_create_file(&pdev->dev,
        &dev_attr_value);

    if (ret) {
        dev_err(&pdev->dev,
            "Failed to create sysfs file\n");

        return ret;
    }

    dev_info(&pdev->dev,
        "GPIO sysfs attribute created\n");

    return 0;
}

//-------------------------------------------

static int gpio_sysfs_remove(struct platform_device* pdev)
{
    device_remove_file(&pdev->dev,
        &dev_attr_value);

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpio_sysfs_match[] = {
    {
        .compatible = "ldd,gpio-sysfs",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_sysfs_match);

//-------------------------------------------

static struct platform_driver gpio_sysfs_driver = {
    .probe = gpio_sysfs_probe,
    .remove = gpio_sysfs_remove,

    .driver = {
        .name = "ldd_gpio_sysfs",
        .of_match_table = gpio_sysfs_match,
    },
};

//-------------------------------------------

module_platform_driver(gpio_sysfs_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO sysfs example");



