#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/gpio/consumer.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>

//-------------------------------------------

#define DEBOUNCE_MS    50

//-------------------------------------------

struct gpio_debounce_data {
    struct gpio_desc* gpio;
    int irq;

    struct delayed_work debounce_work;
};

//-------------------------------------------

static void debounce_work_handler(struct work_struct* work)
{
    struct gpio_debounce_data* data;

    data = container_of(to_delayed_work(work),
        struct gpio_debounce_data,
        debounce_work);

    if (gpiod_get_value_cansleep(data->gpio))
        pr_info("Button state: PRESSED\n");
    else
        pr_info("Button state: RELEASED\n");
}

//-------------------------------------------

static irqreturn_t debounce_irq_handler(int irq,
    void* dev_id)
{
    struct gpio_debounce_data* data = dev_id;

    /*
     * Do not process the button immediately.
     *
     * Wait for the electrical bouncing to settle.
     */

    mod_delayed_work(system_wq,
        &data->debounce_work,
        msecs_to_jiffies(DEBOUNCE_MS));

    return IRQ_HANDLED;
}

//-------------------------------------------

static int gpio_debounce_probe(struct platform_device* pdev)
{
    struct gpio_debounce_data* data;
    int ret;

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // GPIO
    //-------------------------------------------

    data->gpio = devm_gpiod_get(&pdev->dev,
        "button",
        GPIOD_IN);

    if (IS_ERR(data->gpio))
        return PTR_ERR(data->gpio);

    //-------------------------------------------
    // GPIO -> IRQ
    //-------------------------------------------

    data->irq = gpiod_to_irq(data->gpio);

    if (data->irq < 0)
        return data->irq;

    //-------------------------------------------
    // Delayed work
    //-------------------------------------------

    INIT_DELAYED_WORK(&data->debounce_work,
        debounce_work_handler);

    //-------------------------------------------
    // IRQ
    //-------------------------------------------

    ret = devm_request_irq(&pdev->dev,
        data->irq,
        debounce_irq_handler,
        IRQF_TRIGGER_RISING |
        IRQF_TRIGGER_FALLING,
        "ldd_gpio_debounce",
        data);

    if (ret) {
        dev_err(&pdev->dev,
            "Failed to request GPIO IRQ\n");

        return ret;
    }

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "GPIO debounce driver loaded\n");

    return 0;
}

//-------------------------------------------

static int gpio_debounce_remove(struct platform_device* pdev)
{
    struct gpio_debounce_data* data;

    data = platform_get_drvdata(pdev);

    if (data)
        cancel_delayed_work_sync(&data->debounce_work);

    return 0;
}

//-------------------------------------------

static const struct of_device_id gpio_debounce_match[] = {
    {
        .compatible = "ldd,gpio-debounce",
    },
    { }
};

MODULE_DEVICE_TABLE(of, gpio_debounce_match);

//-------------------------------------------

static struct platform_driver gpio_debounce_driver = {
    .probe = gpio_debounce_probe,
    .remove = gpio_debounce_remove,

    .driver = {
        .name = "ldd_gpio_debounce",
        .of_match_table = gpio_debounce_match,
    },
};

//-------------------------------------------

module_platform_driver(gpio_debounce_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO software debounce example");


/*
//-------------------------------------------

                  Button
                    │
              electrical bounce
                    │
              ┌─────┴─────┐
              │   IRQ     │
              └─────┬─────┘
                    │
                    ↓
          debounce_irq_handler()
                    │
                    │ schedule after 50 ms
                    ↓
              delayed_work
                    │
                    ↓
          read GPIO after bounce
                    │
                    ↓
             stable button state

//-------------------------------------------
*/

