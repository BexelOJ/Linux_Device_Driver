#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/i2c.h>

#define DRIVER_NAME "ldd_20261026_rpiTemperatureDriver"

struct rpi_temperature_data {
    struct i2c_client* client;
};

static int rpiTemperatureDriver_probe(
    struct i2c_client* client)
{
    struct rpi_temperature_data* data;

    data = devm_kzalloc(&client->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->client = client;

    i2c_set_clientdata(client, data);

    dev_info(&client->dev,
        "%s: temperature sensor detected\n",
        DRIVER_NAME);

    dev_info(&client->dev,
        "I2C address = 0x%02x\n",
        client->addr);

    return 0;
}

static void rpiTemperatureDriver_remove(
    struct i2c_client* client)
{
    dev_info(&client->dev,
        "%s: temperature sensor removed\n",
        DRIVER_NAME);
}

static const struct of_device_id
rpiTemperatureDriver_of_match[] = {
    {
        .compatible = "bexel,rpi-temperature",
    },
    { }
};

MODULE_DEVICE_TABLE(of,
    rpiTemperatureDriver_of_match);

static struct i2c_driver rpiTemperatureDriver_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table =
            rpiTemperatureDriver_of_match,
    },

    .probe = rpiTemperatureDriver_probe,
    .remove = rpiTemperatureDriver_remove,
};

module_i2c_driver(rpiTemperatureDriver_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Raspberry Pi I2C temperature driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


