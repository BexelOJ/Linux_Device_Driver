#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

//-------------------------------------------

struct ldd_regulator_data
{
    struct regulator* vdd;
};

//-------------------------------------------

static int ldd_regulator_probe(struct platform_device* pdev)
{
    struct ldd_regulator_data* data;

    pr_info("regulator: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->vdd = devm_regulator_get_optional(&pdev->dev, "vdd");

    if (IS_ERR(data->vdd))
    {
        if (PTR_ERR(data->vdd) == -ENODEV)
        {
            dev_info(&pdev->dev,
                "vdd regulator is not present\n");

            return 0;
        }

        return PTR_ERR(data->vdd);
    }

    dev_info(&pdev->dev,
        "vdd regulator acquired\n");

    /*
     * We intentionally do not enable or change
     * the regulator here.
     *
     * Actual voltage configuration must be
     * specific to the hardware.
     */

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void ldd_regulator_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "regulator: remove()\n");
}

//-------------------------------------------

static const struct of_device_id ldd_regulator_of_match[] =
{
    {
        .compatible = "ldd,regulator-demo",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_regulator_of_match);

//-------------------------------------------

static struct platform_driver ldd_regulator_driver =
{
    .probe = ldd_regulator_probe,
    .remove = ldd_regulator_remove,

    .driver =
    {
        .name = "ldd-regulator",
        .of_match_table = ldd_regulator_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_regulator_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Regulator Framework example");


/*
//-------------------------------------------

vdd-supply
     │
     ▼
regulator framework
     │
     ▼
devm_regulator_get_optional()
     │
     ▼
struct regulator *

//-------------------------------------------
*/


