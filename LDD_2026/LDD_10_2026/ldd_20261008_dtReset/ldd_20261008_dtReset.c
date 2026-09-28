#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/reset.h>

//-------------------------------------------

static int dt_reset_probe(struct platform_device* pdev)
{
    struct reset_control* reset;
    int ret;

    reset = devm_reset_control_get_exclusive(&pdev->dev,
        "core");

    if (IS_ERR(reset)) {
        dev_err(&pdev->dev,
            "Failed to get reset controller\n");

        return PTR_ERR(reset);
    }

    dev_info(&pdev->dev,
        "Reset controller obtained\n");

    //-------------------------------------------
    // Assert reset
    //-------------------------------------------

    ret = reset_control_assert(reset);

    if (ret) {
        dev_err(&pdev->dev,
            "Failed to assert reset\n");

        return ret;
    }

    dev_info(&pdev->dev,
        "Reset asserted\n");

    //-------------------------------------------
    // Deassert reset
    //-------------------------------------------

    ret = reset_control_deassert(reset);

    if (ret) {
        dev_err(&pdev->dev,
            "Failed to deassert reset\n");

        return ret;
    }

    dev_info(&pdev->dev,
        "Reset deasserted\n");

    return 0;
}

//-------------------------------------------

static const struct of_device_id dt_reset_match[] = {
    {
        .compatible = "ldd,reset-demo",
    },
    { }
};

MODULE_DEVICE_TABLE(of, dt_reset_match);

//-------------------------------------------

static struct platform_driver dt_reset_driver = {
    .probe = dt_reset_probe,

    .driver = {
        .name = "ldd_dt_reset",
        .of_match_table = dt_reset_match,
    },
};

//-------------------------------------------

module_platform_driver(dt_reset_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("er Bexel O J");
MODULE_DESCRIPTION("Device Tree reset controller example");



