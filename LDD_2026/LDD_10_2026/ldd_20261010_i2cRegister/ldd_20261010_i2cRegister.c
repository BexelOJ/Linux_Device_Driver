#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>

//-------------------------------------------

static int i2c_register_probe(struct i2c_client* client)
{
    s32 value;

    //-------------------------------------------
    // Write register
    //-------------------------------------------

    value = i2c_smbus_write_byte_data(client,
        0x10,
        0x55);

    if (value < 0) {
        dev_err(&client->dev,
            "SMBus register write failed\n");

        return value;
    }

    //-------------------------------------------
    // Read register
    //-------------------------------------------

    value = i2c_smbus_read_byte_data(client,
        0x10);

    if (value < 0) {
        dev_err(&client->dev,
            "SMBus register read failed\n");

        return value;
    }

    dev_info(&client->dev,
        "Register 0x10 = 0x%02x\n",
        value);

    return 0;
}

//-------------------------------------------

static void i2c_register_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C register driver removed\n");
}

//-------------------------------------------

static const struct of_device_id i2c_register_match[] = {
    {
        .compatible = "ldd,i2c-register",
    },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_register_match);

//-------------------------------------------

static struct i2c_driver i2c_register_driver = {
    .driver = {
        .name = "ldd_i2c_register",
        .of_match_table = i2c_register_match,
    },

    .probe = i2c_register_probe,
    .remove = i2c_register_remove,
};

//-------------------------------------------

module_i2c_driver(i2c_register_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("I2C register access example");


/*
//-------------------------------------------



//-------------------------------------------
*/


