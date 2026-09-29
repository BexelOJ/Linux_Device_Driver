#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>

#define DRIVER_NAME "ldd_20261026_rpiButtonDriver"

struct rpi_button_data {
    struct gpio_desc* button;
    int irq;
};

static irqreturn_t rpiButtonDriver_irq(int irq,
    void* data)
{
    struct rpi_button_data* button_data = data;

    dev_info(button_data->button->gpiod.dev,
        "%s: button interrupt\n",
        DRIVER_NAME);

    return IRQ_HANDLED;
}

static int rpiButtonDriver_probe(struct platform_device* pdev)
{
    struct rpi_button_data* data;
    int ret;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

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
        rpiButtonDriver_irq,
        IRQF_TRIGGER_RISING |
        IRQF_TRIGGER_FALLING,
        DRIVER_NAME,
        data);

    if (ret)
        return ret;

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "%s: button IRQ=%d\n",
        DRIVER_NAME,
        data->irq);

    return 0;
}

static void rpiButtonDriver_remove(
    struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "%s: button driver removed\n",
        DRIVER_NAME);
}

static const struct of_device_id rpiButtonDriver_of_match[] = {
    {
        .compatible = "bexel,rpi-button-driver",
    },
    { }
};

MODULE_DEVICE_TABLE(of, rpiButtonDriver_of_match);

static struct platform_driver rpiButtonDriver_driver = {
    .probe = rpiButtonDriver_probe,
    .remove = rpiButtonDriver_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = rpiButtonDriver_of_match,
    },
};

module_platform_driver(rpiButtonDriver_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Raspberry Pi GPIO button driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


