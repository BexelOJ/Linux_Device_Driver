#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/gpio/consumer.h>

//-------------------------------------------

struct gpio_irq_data {
    struct gpio_desc* gpio;
    int irq;
};

//-------------------------------------------

static irqreturn_t gpio_irq_handler(int irq, void* data)
{
    struct gpio_irq_data* drvdata = data;

    pr_info("GPIO interrupt received: IRQ=%d\n",
        drvdata->irq);

    return IRQ_HANDLED;
}

//-------------------------------------------

static int gpio_irq_probe(struct platform_device* pdev)
{
    struct gpio_irq_data* data;
    int ret;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Get GPIO
    //-------------------------------------------

    data->gpio = devm_gpiod_get(&pdev->dev,
        "button",
        GPIOD_IN);

    if (IS_ERR(data->gpio))
        return PTR_ERR(data->gpio);

    //-------------------------------------------
    // Convert GPIO to IRQ
    //-------------------------------------------

    data->irq = gpiod_to_irq(data->gpio);

    if (data->irq < 0) {
        dev_err(&pdev->dev,
            "gpiod_to_irq() failed\n");

        return data->irq;
    }

    dev_info(&pdev->dev,
        "GPIO IRQ = %d\n",
        data->irq);

    //-------------------------------------------
    // Request IRQ
    //-------------------------------------------

    ret = devm_request_irq(&pdev->dev,
        data->irq,
        gpio_irq_handler,
        IRQF_TRIGGER_RISING |
        IRQF_TRIGGER_FALLING,
        "ldd_gpio_irq",
        data);

    if (ret) {
        dev_err(&pdev->dev,
            "request_irq() failed\n");

        return ret;
    }

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpio_irq_match[] = {
    {
        .compatible = "ldd,gpio-interrupt",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_irq_match);

//-------------------------------------------

static struct platform_driver gpio_irq_driver = {
    .probe = gpio_irq_probe,

    .driver = {
        .name = "ldd_gpio_interrupt",
        .of_match_table = gpio_irq_match,
    },
};

//-------------------------------------------

module_platform_driver(gpio_irq_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO interrupt example");



