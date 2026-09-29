#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/i2c.h>

#define DRIVER_NAME "ldd_20261024_embeddedI2C"

static int embeddedI2C_probe(struct i2c_client* client)
{
    pr_info("%s: I2C device detected\n",
        DRIVER_NAME);

    pr_info("%s: address=0x%02x\n",
        DRIVER_NAME,
        client->addr);

    return 0;
}

static void embeddedI2C_remove(struct i2c_client* client)
{
    pr_info("%s: I2C device removed\n",
        DRIVER_NAME);
}

static const struct of_device_id embeddedI2C_of_match[] = {
    {
        .compatible = "bexel,embedded-i2c",
    },
    { }
};

MODULE_DEVICE_TABLE(of, embeddedI2C_of_match);

static struct i2c_driver embeddedI2C_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = embeddedI2C_of_match,
    },
    .probe = embeddedI2C_probe,
    .remove = embeddedI2C_remove,
};

module_i2c_driver(embeddedI2C_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux I2C driver example");


/*
//-------------------------------------------



//-------------------------------------------
*/


