#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>

//-------------------------------------------

static int i2c_client_probe(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C client detected\n");

    //-------------------------------------------
    // I2C address
    //-------------------------------------------

    dev_info(&client->dev,
        "Address : 0x%02x\n",
        client->addr);

    //-------------------------------------------
    // Adapter
    //-------------------------------------------

    dev_info(&client->dev,
        "Adapter : %s\n",
        client->adapter->name);

    //-------------------------------------------
    // Device name
    //-------------------------------------------

    dev_info(&client->dev,
        "Name    : %s\n",
        client->name);

    //-------------------------------------------
    // IRQ if one exists
    //-------------------------------------------

    dev_info(&client->dev,
        "IRQ     : %d\n",
        client->irq);

    return 0;
}

//-------------------------------------------

static void i2c_client_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C client removed\n");
}

//-------------------------------------------

static const struct of_device_id i2c_client_match[] = {
    {
        .compatible = "ldd,i2c-client",
    },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_client_match);

//-------------------------------------------

static struct i2c_driver i2c_client_driver = {
    .driver = {
        .name = "ldd_i2c_client",
        .of_match_table = i2c_client_match,
    },

    .probe = i2c_client_probe,
    .remove = i2c_client_remove,
};

//-------------------------------------------

module_i2c_driver(i2c_client_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("I2C client structure example");


/*
//-------------------------------------------



//-------------------------------------------
*/


