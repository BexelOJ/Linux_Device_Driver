#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/clk.h>

//-------------------------------------------

static int dt_clock_probe(struct platform_device* pdev)
{
    struct clk* clk;
    int ret;

    clk = devm_clk_get(&pdev->dev, "core");

    if (IS_ERR(clk)) {
        dev_err(&pdev->dev,
            "Failed to get clock\n");

        return PTR_ERR(clk);
    }

    dev_info(&pdev->dev,
        "Clock obtained successfully\n");

    ret = clk_prepare_enable(clk);

    if (ret) {
        dev_err(&pdev->dev,
            "Failed to enable clock\n");

        return ret;
    }

    dev_info(&pdev->dev,
        "Clock enabled\n");

    clk_disable_unprepare(clk);

    return 0;
}

//-------------------------------------------

static const struct of_device_id dt_clock_match[] = {
    {
        .compatible = "ldd,clock-demo",
    },
    { }
};

MODULE_DEVICE_TABLE(of, dt_clock_match);

//-------------------------------------------

static struct platform_driver dt_clock_driver = {
    .probe = dt_clock_probe,

    .driver = {
        .name = "ldd_dt_clock",
        .of_match_table = dt_clock_match,
    },
};

//-------------------------------------------

module_platform_driver(dt_clock_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Tree clock example");


/*
//-------------------------------------------



//-------------------------------------------
*/


