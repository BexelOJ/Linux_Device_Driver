#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>

//-------------------------------------------

static int i2c_basic_probe(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C device detected\n");

    dev_info(&client->dev,
        "Address = 0x%02x\n",
        client->addr);

    dev_info(&client->dev,
        "Adapter = %s\n",
        client->adapter->name);

    return 0;
}

//-------------------------------------------

static void i2c_basic_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C device removed\n");
}

//-------------------------------------------

static const struct of_device_id i2c_basic_match[] = {
    {
        .compatible = "ldd,i2c-basic",
    },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_basic_match);

//-------------------------------------------

static struct i2c_driver i2c_basic_driver = {
    .driver = {
        .name = "ldd_i2c_basic",
        .of_match_table = i2c_basic_match,
    },

    .probe = i2c_basic_probe,
    .remove = i2c_basic_remove,
};

//-------------------------------------------

module_i2c_driver(i2c_basic_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic I2C driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


