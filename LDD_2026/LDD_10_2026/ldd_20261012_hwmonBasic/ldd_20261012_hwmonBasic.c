#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>

/*
 * Basic hwmon driver.
 *
 * Exposes:
 *
 *     /sys/class/hwmon/hwmonX/
 */

 //-------------------------------------------

static ssize_t temp1_input_show(struct device* dev,
    struct device_attribute* attr,
    char* buf)
{
    /*
     * Temperature is expressed in
     * millidegrees Celsius.
     *
     * 25000 = 25 degrees Celsius.
     */

    return sysfs_emit(buf, "25000\n");
}

//-------------------------------------------

static DEVICE_ATTR_RO(temp1_input);

//-------------------------------------------

static struct attribute* hwmon_basic_attrs[] =
{
    &dev_attr_temp1_input.attr,
    NULL,
};

ATTRIBUTE_GROUPS(hwmon_basic);

//-------------------------------------------

static int hwmon_basic_probe(struct platform_device* pdev)
{
    struct device* hwmon;

    pr_info("hwmonBasic: probe()\n");

    hwmon = devm_hwmon_device_register_with_groups(
        &pdev->dev,
        "ldd_hwmon",
        NULL,
        hwmon_basic_groups);

    if (IS_ERR(hwmon))
        return PTR_ERR(hwmon);

    pr_info("hwmonBasic: hwmon registered\n");

    return 0;
}

//-------------------------------------------

static void hwmon_basic_remove(struct platform_device* pdev)
{
    pr_info("hwmonBasic: remove()\n");
}

//-------------------------------------------

static const struct of_device_id hwmon_basic_of_match[] =
{
    {
        .compatible = "ldd,hwmon-basic",
    },
    { }
};

MODULE_DEVICE_TABLE(of, hwmon_basic_of_match);

//-------------------------------------------

static struct platform_driver hwmon_basic_driver =
{
    .probe = hwmon_basic_probe,
    .remove = hwmon_basic_remove,

    .driver =
    {
        .name = "ldd_hwmon_basic",
        .of_match_table = hwmon_basic_of_match,
    },
};

//-------------------------------------------

module_platform_driver(hwmon_basic_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic hwmon driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


