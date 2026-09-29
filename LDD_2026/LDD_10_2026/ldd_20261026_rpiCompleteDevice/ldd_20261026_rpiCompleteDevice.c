#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>

#define DRIVER_NAME "ldd_20261026_rpiCompleteDevice"

struct rpi_complete_data {
    struct gpio_desc* led;
    struct gpio_desc* button;
    int irq;
};

static irqreturn_t rpiCompleteDevice_irq(
    int irq,
    void* data)
{
    struct rpi_complete_data* driver_data = data;

    gpiod_set_value_cansleep(driver_data->led, 1);

    pr_info("%s: button IRQ received\n",
        DRIVER_NAME);

    return IRQ_HANDLED;
}

static int rpiCompleteDevice_probe(
    struct platform_device* pdev)
{
    struct rpi_complete_data* data;
    int ret;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->led =
        devm_gpiod_get(&pdev->dev,
            "led",
            GPIOD_OUT_LOW);

    if (IS_ERR(data->led))
        return PTR_ERR(data->led);

    data->button =
        devm_gpiod_get(&pdev->dev,
            "button",
            GPIOD_IN);

    if (IS_ERR(data->button))
        return PTR_ERR(data->button);

    data->irq = gpiod_to_irq(data->button);

    if (data->irq < 0)
        return data->irq;

    ret = devm_request_irq(&pdev->dev,
        data->irq,
        rpiCompleteDevice_irq,
        IRQF_TRIGGER_RISING,
        DRIVER_NAME,
        data);

    if (ret)
        return ret;

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "%s: complete device initialized\n",
        DRIVER_NAME);

    return 0;
}

static void rpiCompleteDevice_remove(
    struct platform_device* pdev)
{
    struct rpi_complete_data* data;

    data = platform_get_drvdata(pdev);

    gpiod_set_value_cansleep(data->led, 0);

    dev_info(&pdev->dev,
        "%s: complete device removed\n",
        DRIVER_NAME);
}

static const struct of_device_id rpiCompleteDevice_of_match[] = {
    {
        .compatible = "bexel,rpi-complete-device",
    },
    { }
};

MODULE_DEVICE_TABLE(of, rpiCompleteDevice_of_match);

static struct platform_driver rpiCompleteDevice_driver = {
    .probe = rpiCompleteDevice_probe,
    .remove = rpiCompleteDevice_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table =
            rpiCompleteDevice_of_match,
    },
};

module_platform_driver(rpiCompleteDevice_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete Raspberry Pi device driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


