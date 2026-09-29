#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>

#define DRIVER_NAME "ldd_20261024_embeddedComplete"

struct ldd_complete_data {
    struct gpio_desc* gpio;
    int irq;
};

static irqreturn_t embeddedComplete_irq(
    int irq,
    void* data)
{
    struct ldd_complete_data* driver_data = data;

    pr_info("%s: interrupt received, irq=%d\n",
        DRIVER_NAME,
        driver_data->irq);

    return IRQ_HANDLED;
}

static int embeddedComplete_probe(
    struct platform_device* pdev)
{
    struct ldd_complete_data* driver_data;
    int ret;

    driver_data = devm_kzalloc(&pdev->dev,
        sizeof(*driver_data),
        GFP_KERNEL);

    if (!driver_data)
        return -ENOMEM;

    driver_data->gpio =
        devm_gpiod_get(&pdev->dev,
            "status",
            GPIOD_OUT_LOW);

    if (IS_ERR(driver_data->gpio))
        return PTR_ERR(driver_data->gpio);

    driver_data->irq =
        platform_get_irq(pdev, 0);

    if (driver_data->irq < 0)
        return driver_data->irq;

    ret = devm_request_irq(&pdev->dev,
        driver_data->irq,
        embeddedComplete_irq,
        0,
        DRIVER_NAME,
        driver_data);

    if (ret)
        return ret;

    platform_set_drvdata(pdev, driver_data);

    gpiod_set_value_cansleep(driver_data->gpio, 1);

    pr_info("%s: complete driver initialized\n",
        DRIVER_NAME);

    return 0;
}

static void embeddedComplete_remove(
    struct platform_device* pdev)
{
    struct ldd_complete_data* driver_data;

    driver_data = platform_get_drvdata(pdev);

    gpiod_set_value_cansleep(driver_data->gpio, 0);

    pr_info("%s: driver removed\n", DRIVER_NAME);
}

static const struct of_device_id embeddedComplete_of_match[] = {
    {
        .compatible = "bexel,embedded-complete",
    },
    { }
};

MODULE_DEVICE_TABLE(of, embeddedComplete_of_match);

static struct platform_driver embeddedComplete_driver = {
    .probe = embeddedComplete_probe,
    .remove = embeddedComplete_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table =
            embeddedComplete_of_match,
    },
};

module_platform_driver(embeddedComplete_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete embedded Linux driver example");


/*
//-------------------------------------------



//-------------------------------------------
*/


