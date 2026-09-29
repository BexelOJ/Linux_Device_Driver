#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

//-------------------------------------------

struct ldd_input_data {
    struct input_dev* input;

    struct gpio_desc* button_gpio;

    int irq;

    struct delayed_work debounce_work;
};

//-------------------------------------------

static void ldd_input_debounce_work(struct work_struct* work)
{
    struct ldd_input_data* data;
    int value;

    data = container_of(to_delayed_work(work),
        struct ldd_input_data,
        debounce_work);

    //-------------------------------------------
    // Read GPIO after debounce delay
    //-------------------------------------------

    value = gpiod_get_value_cansleep(
        data->button_gpio);

    //-------------------------------------------
    // Report key state
    //-------------------------------------------

    input_report_key(data->input,
        KEY_ENTER,
        value);

    input_sync(data->input);

    dev_info(data->input->dev.parent,
        "button state = %d\n",
        value);
}

//-------------------------------------------

static irqreturn_t ldd_input_irq_handler(int irq,
    void* dev_id)
{
    struct ldd_input_data* data = dev_id;

    /*
     * Do not perform debounce work directly
     * inside the IRQ handler.
     */

    mod_delayed_work(system_wq,
        &data->debounce_work,
        msecs_to_jiffies(30));

    return IRQ_HANDLED;
}

//-------------------------------------------

static int ldd_input_probe(struct platform_device* pdev)
{
    struct ldd_input_data* data;
    int ret;

    dev_info(&pdev->dev,
        "ldd_inputComplete: probe\n");

    //-------------------------------------------
    // Allocate driver data
    //-------------------------------------------

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    platform_set_drvdata(pdev, data);

    //-------------------------------------------
    // Get GPIO
    //-------------------------------------------

    data->button_gpio =
        devm_gpiod_get(&pdev->dev,
            "button",
            GPIOD_IN);

    if (IS_ERR(data->button_gpio)) {
        dev_err(&pdev->dev,
            "failed to get button GPIO\n");

        return PTR_ERR(data->button_gpio);
    }

    //-------------------------------------------
    // Allocate input device
    //-------------------------------------------

    data->input =
        devm_input_allocate_device(&pdev->dev);

    if (!data->input)
        return -ENOMEM;

    //-------------------------------------------
    // Configure input device
    //-------------------------------------------

    data->input->name = "ldd-complete-input";
    data->input->phys = "ldd/input-complete";
    data->input->id.bustype = BUS_HOST;

    input_set_capability(data->input,
        EV_KEY,
        KEY_ENTER);

    //-------------------------------------------
    // Register input device
    //-------------------------------------------

    ret = input_register_device(data->input);

    if (ret) {
        dev_err(&pdev->dev,
            "input_register_device failed\n");

        return ret;
    }

    //-------------------------------------------
    // Convert GPIO to IRQ
    //-------------------------------------------

    data->irq = gpiod_to_irq(data->button_gpio);

    if (data->irq < 0) {
        dev_err(&pdev->dev,
            "gpiod_to_irq failed\n");

        return data->irq;
    }

    //-------------------------------------------
    // Initialize debounce work
    //-------------------------------------------

    INIT_DELAYED_WORK(&data->debounce_work,
        ldd_input_debounce_work);

    //-------------------------------------------
    // Request IRQ
    //-------------------------------------------

    ret = devm_request_irq(&pdev->dev,
        data->irq,
        ldd_input_irq_handler,
        IRQF_TRIGGER_RISING |
        IRQF_TRIGGER_FALLING,
        "ldd_inputComplete",
        data);

    if (ret) {
        dev_err(&pdev->dev,
            "request_irq failed\n");

        return ret;
    }

    //-------------------------------------------

    dev_info(&pdev->dev,
        "complete input driver registered\n");

    dev_info(&pdev->dev,
        "IRQ = %d\n",
        data->irq);

    return 0;
}

//-------------------------------------------

static void ldd_input_remove(struct platform_device* pdev)
{
    struct ldd_input_data* data;

    data = platform_get_drvdata(pdev);

    cancel_delayed_work_sync(&data->debounce_work);

    dev_info(&pdev->dev,
        "ldd_inputComplete: remove\n");
}

//-------------------------------------------

static const struct of_device_id
ldd_input_of_match[] = {

    {
        .compatible = "ldd,input-complete",
    },

    { }
};

MODULE_DEVICE_TABLE(of, ldd_input_of_match);

//-------------------------------------------

static struct platform_driver ldd_input_driver = {

    .probe = ldd_input_probe,
    .remove = ldd_input_remove,

    .driver = {
        .name = "ldd-input-complete",

        .of_match_table =
            ldd_input_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_input_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete GPIO Input driver");


/*


Device Tree
     ↓
GPIO
     ↓
IRQ
     ↓
Debounce
     ↓
Input subsystem
     ↓
/dev/input/eventX


*/


