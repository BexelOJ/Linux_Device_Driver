#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>

//-------------------------------------------

static int i2c_driver_probe(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C driver probe()\n");

    dev_info(&client->dev,
        "I2C address = 0x%02x\n",
        client->addr);

    return 0;
}

//-------------------------------------------

static void i2c_driver_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C driver remove()\n");
}

//-------------------------------------------

static const struct of_device_id i2c_driver_match[] = {
    {
        .compatible = "ldd,i2c-driver",
    },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_driver_match);

//-------------------------------------------

static struct i2c_driver my_i2c_driver = {
    .driver = {
        .name = "ldd_i2c_driver",
        .of_match_table = i2c_driver_match,
    },

    .probe = i2c_driver_probe,
    .remove = i2c_driver_remove,
};

//-------------------------------------------

module_i2c_driver(my_i2c_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic I2C driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


