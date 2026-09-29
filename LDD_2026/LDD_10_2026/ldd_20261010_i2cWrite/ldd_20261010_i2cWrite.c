#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>

//-------------------------------------------

static int i2c_write_probe(struct i2c_client* client)
{
    u8 buffer[2];
    int ret;

    /*
     * Example:
     *
     * buffer[0] = register address
     * buffer[1] = value
     */

    buffer[0] = 0x10;
    buffer[1] = 0x55;

    ret = i2c_master_send(client,
        buffer,
        sizeof(buffer));

    if (ret < 0) {
        dev_err(&client->dev,
            "I2C write failed: %d\n",
            ret);

        return ret;
    }

    dev_info(&client->dev,
        "I2C write successful\n");

    return 0;
}

//-------------------------------------------

static void i2c_write_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C write driver removed\n");
}

//-------------------------------------------

static const struct of_device_id i2c_write_match[] = {
    {
        .compatible = "ldd,i2c-write",
    },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_write_match);

//-------------------------------------------

static struct i2c_driver i2c_write_driver = {
    .driver = {
        .name = "ldd_i2c_write",
        .of_match_table = i2c_write_match,
    },

    .probe = i2c_write_probe,
    .remove = i2c_write_remove,
};

//-------------------------------------------

module_i2c_driver(i2c_write_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("I2C write example");


/*
//-------------------------------------------



//-------------------------------------------
*/


