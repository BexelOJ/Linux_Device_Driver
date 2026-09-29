#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

#define DRIVER_NAME "ldd_20261026_rpiLEDDriver"

struct rpi_led_data {
    struct gpio_desc* led;
};

static int rpiLEDDriver_probe(struct platform_device* pdev)
{
    struct rpi_led_data* data;

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

    gpiod_set_value_cansleep(data->led, 1);

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "%s: LED turned ON\n",
        DRIVER_NAME);

    return 0;
}

static void rpiLEDDriver_remove(struct platform_device* pdev)
{
    struct rpi_led_data* data;

    data = platform_get_drvdata(pdev);

    gpiod_set_value_cansleep(data->led, 0);

    dev_info(&pdev->dev,
        "%s: LED turned OFF\n",
        DRIVER_NAME);
}

static const struct of_device_id rpiLEDDriver_of_match[] = {
    {
        .compatible = "bexel,rpi-led-driver",
    },
    { }
};

MODULE_DEVICE_TABLE(of, rpiLEDDriver_of_match);

static struct platform_driver rpiLEDDriver_driver = {
    .probe = rpiLEDDriver_probe,
    .remove = rpiLEDDriver_remove,

    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = rpiLEDDriver_of_match,
    },
};

module_platform_driver(rpiLEDDriver_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Raspberry Pi LED GPIO driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


