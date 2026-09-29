#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>

//-------------------------------------------

static int i2c_probe_probe(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C probe successful\n");

    dev_info(&client->dev,
        "Device address = 0x%02x\n",
        client->addr);

    return 0;
}

//-------------------------------------------

static void i2c_probe_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C probe driver removed\n");
}

//-------------------------------------------

static const struct of_device_id i2c_probe_match[] = {
    {
        .compatible = "ldd,i2c-probe",
    },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_probe_match);

//-------------------------------------------

static struct i2c_driver i2c_probe_driver = {
    .driver = {
        .name = "ldd_i2c_probe",
        .of_match_table = i2c_probe_match,
    },

    .probe = i2c_probe_probe,
    .remove = i2c_probe_remove,
};

//-------------------------------------------

module_i2c_driver(i2c_probe_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("I2C probe example");


/*
//-------------------------------------------



//-------------------------------------------
*/


