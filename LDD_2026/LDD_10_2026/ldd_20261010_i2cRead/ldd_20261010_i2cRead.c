#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>

//-------------------------------------------

static int i2c_read_probe(struct i2c_client* client)
{
    u8 buffer[2];
    int ret;

    ret = i2c_master_recv(client,
        buffer,
        sizeof(buffer));

    if (ret < 0) {
        dev_err(&client->dev,
            "I2C read failed: %d\n",
            ret);

        return ret;
    }

    dev_info(&client->dev,
        "Read %d bytes\n",
        ret);

    if (ret >= 2) {
        dev_info(&client->dev,
            "Data: 0x%02x 0x%02x\n",
            buffer[0],
            buffer[1]);
    }

    return 0;
}

//-------------------------------------------

static void i2c_read_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C read driver removed\n");
}

//-------------------------------------------

static const struct of_device_id i2c_read_match[] = {
    {
        .compatible = "ldd,i2c-read",
    },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_read_match);

//-------------------------------------------

static struct i2c_driver i2c_read_driver = {
    .driver = {
        .name = "ldd_i2c_read",
        .of_match_table = i2c_read_match,
    },

    .probe = i2c_read_probe,
    .remove = i2c_read_remove,
};

//-------------------------------------------

module_i2c_driver(i2c_read_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("I2C read example");


/*
//-------------------------------------------



//-------------------------------------------
*/


