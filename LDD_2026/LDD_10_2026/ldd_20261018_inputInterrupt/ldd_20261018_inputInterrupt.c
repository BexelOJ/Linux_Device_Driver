#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/gpio/consumer.h>

//-------------------------------------------

struct ldd_gpio_input {
    struct input_dev* input;
};

static struct ldd_gpio_input* data;

//-------------------------------------------

static int __init ldd_inputGPIO_init(void)
{
    int ret;

    pr_info("ldd_inputGPIO: init\n");

    //-------------------------------------------
    // Allocate private data
    //-------------------------------------------

    data = kzalloc(sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Allocate input device
    //-------------------------------------------

    data->input = input_allocate_device();

    if (!data->input) {
        kfree(data);
        return -ENOMEM;
    }

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    data->input->name = "ldd-gpio-input";
    data->input->phys = "ldd/gpio0";
    data->input->id.bustype = BUS_HOST;

    input_set_capability(data->input,
        EV_KEY,
        KEY_ENTER);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = input_register_device(data->input);

    if (ret) {
        input_free_device(data->input);
        kfree(data);
        return ret;
    }

    pr_info("ldd_inputGPIO: input device registered\n");

    /*
     * A real GPIO descriptor must be obtained from
     * a struct device, normally in probe():
     *
     * devm_gpiod_get(&pdev->dev,
     *                "button",
     *                GPIOD_IN);
     */

    return 0;
}

//-------------------------------------------

static void __exit ldd_inputGPIO_exit(void)
{
    pr_info("ldd_inputGPIO: exit\n");

    input_unregister_device(data->input);

    kfree(data);
}

//-------------------------------------------

module_init(ldd_inputGPIO_init);
module_exit(ldd_inputGPIO_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("GPIO Input Subsystem example");


/*



*/


