#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/i2c.h>

#define DRIVER_NAME "ldd_20261026_rpiIMUDriver"

struct rpi_imu_data {
    struct i2c_client* client;
};

static int rpiIMUDriver_probe(struct i2c_client* client)
{
    struct rpi_imu_data* data;

    data = devm_kzalloc(&client->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->client = client;

    i2c_set_clientdata(client, data);

    dev_info(&client->dev,
        "%s: IMU detected at 0x%02x\n",
        DRIVER_NAME,
        client->addr);

    return 0;
}

static void rpiIMUDriver_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "%s: IMU removed\n",
        DRIVER_NAME);
}

static const struct of_device_id rpiIMUDriver_of_match[] = {
    {
        .compatible = "bexel,rpi-imu",
    },
    { }
};

MODULE_DEVICE_TABLE(of, rpiIMUDriver_of_match);

static struct i2c_driver rpiIMUDriver_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = rpiIMUDriver_of_match,
    },

    .probe = rpiIMUDriver_probe,
    .remove = rpiIMUDriver_remove,
};

module_i2c_driver(rpiIMUDriver_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Raspberry Pi I2C IMU driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


