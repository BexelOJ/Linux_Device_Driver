#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

//-------------------------------------------

struct ldd_clock_data
{
    struct clk* clk;
};

//-------------------------------------------

static int ldd_clock_probe(struct platform_device* pdev)
{
    struct ldd_clock_data* data;
    unsigned long rate;

    dev_info(&pdev->dev,
        "clockFramework: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->clk = devm_clk_get_optional(&pdev->dev,
        "core");

    if (IS_ERR(data->clk))
        return PTR_ERR(data->clk);

    if (!data->clk)
    {
        dev_info(&pdev->dev,
            "No clock supplied\n");

        return 0;
    }

    rate = clk_get_rate(data->clk);

    dev_info(&pdev->dev,
        "Clock rate = %lu Hz\n",
        rate);

    platform_set_drvdata(pdev, data);

    /*
     * We intentionally do not change the clock
     * rate or enable it in this basic example.
     */

    return 0;
}

//-------------------------------------------

static void ldd_clock_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "clockFramework: remove()\n");
}

//-------------------------------------------

static const struct of_device_id ldd_clock_of_match[] =
{
    {
        .compatible = "ldd,clock-demo",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_clock_of_match);

//-------------------------------------------

static struct platform_driver ldd_clock_driver =
{
    .probe = ldd_clock_probe,
    .remove = ldd_clock_remove,

    .driver =
    {
        .name = "ldd-clock-framework",
        .of_match_table = ldd_clock_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_clock_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Common Clock Framework example");


/*



*/


