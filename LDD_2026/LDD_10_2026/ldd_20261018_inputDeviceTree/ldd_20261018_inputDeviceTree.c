#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/input.h>

//-------------------------------------------

struct ldd_dt_input {
    struct input_dev* input;
    struct gpio_desc* button;
};

//-------------------------------------------

static int ldd_input_probe(struct platform_device* pdev)
{
    struct ldd_dt_input* data;
    int ret;

    dev_info(&pdev->dev,
        "ldd_inputDeviceTree: probe\n");

    //-------------------------------------------
    // Allocate private data
    //-------------------------------------------

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Get GPIO from Device Tree
    //-------------------------------------------

    data->button = devm_gpiod_get(&pdev->dev,
        "button",
        GPIOD_IN);

    if (IS_ERR(data->button)) {
        dev_err(&pdev->dev,
            "failed to get button GPIO\n");

        return PTR_ERR(data->button);
    }

    //-------------------------------------------
    // Allocate input device
    //-------------------------------------------

    data->input =
        devm_input_allocate_device(&pdev->dev);

    if (!data->input)
        return -ENOMEM;

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    data->input->name = "ldd-dt-button";
    data->input->phys = "ldd/dt-button";
    data->input->id.bustype = BUS_HOST;

    input_set_capability(data->input,
        EV_KEY,
        KEY_ENTER);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = input_register_device(data->input);

    if (ret) {
        dev_err(&pdev->dev,
            "input registration failed\n");

        return ret;
    }

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "Device Tree input registered\n");

    return 0;
}

//-------------------------------------------

static void ldd_input_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "ldd_inputDeviceTree: remove\n");
}

//-------------------------------------------

static const struct of_device_id
ldd_input_of_match[] = {

    {
        .compatible = "ldd,input-button",
    },

    { }
};

MODULE_DEVICE_TABLE(of, ldd_input_of_match);

//-------------------------------------------

static struct platform_driver ldd_input_driver = {

    .probe = ldd_input_probe,
    .remove = ldd_input_remove,

    .driver = {
        .name = "ldd-input-dt",

        .of_match_table =
            ldd_input_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_input_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Device Tree Input Button driver");


/*


Device Tree
    │
    │ button-gpios
    ▼
devm_gpiod_get()
    │
    ▼
struct gpio_desc


*/


