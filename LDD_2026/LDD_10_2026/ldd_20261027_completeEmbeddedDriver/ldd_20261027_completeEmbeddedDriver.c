#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>

struct complete_embedded {
    struct gpio_desc* led;
    struct gpio_desc* button;
    int irq;
};

//-------------------------------------------
// Button interrupt
//-------------------------------------------

static irqreturn_t complete_embedded_irq(int irq,
    void* dev_id)
{
    struct complete_embedded* data = dev_id;

    if (gpiod_get_value_cansleep(data->button))
        gpiod_set_value_cansleep(data->led, 1);
    else
        gpiod_set_value_cansleep(data->led, 0);

    return IRQ_HANDLED;
}

//-------------------------------------------
// Probe
//-------------------------------------------

static int complete_embedded_probe(struct platform_device* pdev)
{
    struct complete_embedded* data;
    int ret;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->led = devm_gpiod_get(&pdev->dev,
        "led",
        GPIOD_OUT_LOW);

    if (IS_ERR(data->led))
        return PTR_ERR(data->led);

    data->button = devm_gpiod_get(&pdev->dev,
        "button",
        GPIOD_IN);

    if (IS_ERR(data->button))
        return PTR_ERR(data->button);

    data->irq = gpiod_to_irq(data->button);

    if (data->irq < 0)
        return data->irq;

    ret = devm_request_threaded_irq(&pdev->dev,
        data->irq,
        NULL,
        complete_embedded_irq,
        IRQF_ONESHOT |
        IRQF_TRIGGER_RISING |
        IRQF_TRIGGER_FALLING,
        "complete_embedded",
        data);

    if (ret)
        return ret;

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "complete embedded driver loaded\n");

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void complete_embedded_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "complete embedded driver removed\n");
}

//-------------------------------------------
// Device Tree
//-------------------------------------------

static const struct of_device_id complete_embedded_of_match[] = {
    {.compatible = "bexel,complete-embedded" },
    { }
};

MODULE_DEVICE_TABLE(of, complete_embedded_of_match);

//-------------------------------------------
// Platform driver
//-------------------------------------------

static struct platform_driver complete_embedded_driver = {
    .probe = complete_embedded_probe,
    .remove = complete_embedded_remove,

    .driver = {
        .name = "complete-embedded-driver",
        .of_match_table = complete_embedded_of_match,
    },
};

module_platform_driver(complete_embedded_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete embedded GPIO and IRQ driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


