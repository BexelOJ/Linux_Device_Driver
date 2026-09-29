#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>

//-------------------------------------------

struct ldd_power_data
{
    struct regulator* vdd;
    struct clk* clk;
};

//-------------------------------------------

static int ldd_power_runtime_suspend(struct device* dev)
{
    struct ldd_power_data* data = dev_get_drvdata(dev);

    dev_info(dev,
        "powerComplete: runtime_suspend()\n");

    if (data->clk)
        clk_disable_unprepare(data->clk);

    if (data->vdd)
        regulator_disable(data->vdd);

    return 0;
}

//-------------------------------------------

static int ldd_power_runtime_resume(struct device* dev)
{
    struct ldd_power_data* data = dev_get_drvdata(dev);
    int ret;

    dev_info(dev,
        "powerComplete: runtime_resume()\n");

    if (data->vdd)
    {
        ret = regulator_enable(data->vdd);

        if (ret)
            return ret;
    }

    if (data->clk)
    {
        ret = clk_prepare_enable(data->clk);

        if (ret)
        {
            if (data->vdd)
                regulator_disable(data->vdd);

            return ret;
        }
    }

    return 0;
}

//-------------------------------------------

static int ldd_power_suspend(struct device* dev)
{
    dev_info(dev,
        "powerComplete: system suspend()\n");

    return 0;
}

//-------------------------------------------

static int ldd_power_resume(struct device* dev)
{
    dev_info(dev,
        "powerComplete: system resume()\n");

    return 0;
}

//-------------------------------------------

static const struct dev_pm_ops ldd_power_pm_ops =
{
    .suspend = ldd_power_suspend,
    .resume = ldd_power_resume,

    .runtime_suspend = ldd_power_runtime_suspend,
    .runtime_resume = ldd_power_runtime_resume,
};

//-------------------------------------------

static int ldd_power_probe(struct platform_device* pdev)
{
    struct ldd_power_data* data;
    int ret;

    dev_info(&pdev->dev,
        "powerComplete: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->vdd =
        devm_regulator_get_optional(&pdev->dev, "vdd");

    if (IS_ERR(data->vdd))
    {
        if (PTR_ERR(data->vdd) == -ENODEV)
            data->vdd = NULL;
        else
            return PTR_ERR(data->vdd);
    }

    data->clk =
        devm_clk_get_optional(&pdev->dev, "core");

    if (IS_ERR(data->clk))
        return PTR_ERR(data->clk);

    platform_set_drvdata(pdev, data);

    pm_runtime_set_active(&pdev->dev);
    pm_runtime_enable(&pdev->dev);

    ret = pm_runtime_resume_and_get(&pdev->dev);

    if (ret < 0)
    {
        pm_runtime_disable(&pdev->dev);
        return ret;
    }

    pm_runtime_put_autosuspend(&pdev->dev);
    pm_runtime_set_autosuspend_delay(&pdev->dev, 5000);
    pm_runtime_use_autosuspend(&pdev->dev);

    dev_info(&pdev->dev,
        "powerComplete: PM setup complete\n");

    return 0;
}

//-------------------------------------------

static void ldd_power_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "powerComplete: remove()\n");

    pm_runtime_disable(&pdev->dev);
}

//-------------------------------------------

static const struct of_device_id ldd_power_of_match[] =
{
    {
        .compatible = "ldd,power-complete",
    },
    { }
};

MODULE_DEVICE_TABLE(of, ldd_power_of_match);

//-------------------------------------------

static struct platform_driver ldd_power_driver =
{
    .probe = ldd_power_probe,
    .remove = ldd_power_remove,

    .driver =
    {
        .name = "ldd-power-complete",
        .of_match_table = ldd_power_of_match,
        .pm = &ldd_power_pm_ops,
    },
};

//-------------------------------------------

module_platform_driver(ldd_power_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete Linux device power management example");


/*
//-------------------------------------------



//-------------------------------------------
*/


