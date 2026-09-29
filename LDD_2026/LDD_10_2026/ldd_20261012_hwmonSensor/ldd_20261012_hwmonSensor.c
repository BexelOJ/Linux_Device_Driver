#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>

/*
 * Simple hwmon temperature sensor.
 */

 //-------------------------------------------

static ssize_t temperature_show(struct device* dev,
    struct device_attribute* attr,
    char* buf)
{
    /*
     * hwmon temperature values are normally
     * expressed in millidegrees Celsius.
     *
     * Example:
     *
     * 45000 = 45.000 degrees Celsius
     */

    return sysfs_emit(buf, "%d\n", 45000);
}

//-------------------------------------------

static DEVICE_ATTR_RO(temperature);

//-------------------------------------------

static struct attribute* hwmon_sensor_attrs[] =
{
    &dev_attr_temperature.attr,
    NULL,
};

ATTRIBUTE_GROUPS(hwmon_sensor);

//-------------------------------------------

static int hwmon_sensor_probe(struct platform_device* pdev)
{
    struct device* hwmon_dev;

    pr_info("hwmonSensor: probe()\n");

    hwmon_dev = devm_hwmon_device_register_with_groups(
        &pdev->dev,
        "ldd_sensor",
        NULL,
        hwmon_sensor_groups);

    if (IS_ERR(hwmon_dev))
    {
        pr_err("hwmonSensor: registration failed\n");

        return PTR_ERR(hwmon_dev);
    }

    pr_info("hwmonSensor: hwmon device registered\n");

    return 0;
}

//-------------------------------------------

static void hwmon_sensor_remove(struct platform_device* pdev)
{
    pr_info("hwmonSensor: remove()\n");
}

//-------------------------------------------

static const struct of_device_id hwmon_sensor_of_match[] =
{
    {
        .compatible = "ldd,hwmon-sensor",
    },
    { }
};

MODULE_DEVICE_TABLE(of, hwmon_sensor_of_match);

//-------------------------------------------

static struct platform_driver hwmon_sensor_driver =
{
    .probe = hwmon_sensor_probe,
    .remove = hwmon_sensor_remove,

    .driver =
    {
        .name = "ldd_hwmon_sensor",
        .of_match_table = hwmon_sensor_of_match,
    },
};

//-------------------------------------------

module_platform_driver(hwmon_sensor_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic hwmon temperature sensor");



