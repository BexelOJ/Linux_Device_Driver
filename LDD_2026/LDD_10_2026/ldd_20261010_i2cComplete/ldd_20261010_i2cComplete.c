#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/slab.h>

//-------------------------------------------

struct i2c_complete_data {
    struct i2c_client* client;

    u8 last_value;
};

//-------------------------------------------

static int i2c_complete_write_reg(struct i2c_client* client,
    u8 reg,
    u8 value)
{
    u8 buffer[2];

    buffer[0] = reg;
    buffer[1] = value;

    return i2c_master_send(client,
        buffer,
        sizeof(buffer));
}

//-------------------------------------------

static int i2c_complete_read_reg(struct i2c_client* client,
    u8 reg,
    u8* value)
{
    int ret;

    ret = i2c_master_send(client,
        &reg,
        1);

    if (ret < 0)
        return ret;

    ret = i2c_master_recv(client,
        value,
        1);

    return ret;
}

//-------------------------------------------

static int i2c_complete_probe(struct i2c_client* client)
{
    struct i2c_complete_data* data;
    u8 value;
    int ret;

    data = devm_kzalloc(&client->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->client = client;

    i2c_set_clientdata(client, data);

    dev_info(&client->dev,
        "I2C complete driver probe()\n");

    //-------------------------------------------
    // Example write
    //-------------------------------------------

    ret = i2c_complete_write_reg(client,
        0x10,
        0x55);

    if (ret < 0) {
        dev_err(&client->dev,
            "Register write failed\n");

        return ret;
    }

    //-------------------------------------------
    // Example read
    //-------------------------------------------

    ret = i2c_complete_read_reg(client,
        0x10,
        &value);

    if (ret < 0) {
        dev_err(&client->dev,
            "Register read failed\n");

        return ret;
    }

    data->last_value = value;

    dev_info(&client->dev,
        "Register value = 0x%02x\n",
        value);

    return 0;
}

//-------------------------------------------

static void i2c_complete_remove(struct i2c_client* client)
{
    struct i2c_complete_data* data;

    data = i2c_get_clientdata(client);

    if (data)
        dev_info(&client->dev,
            "Last value = 0x%02x\n",
            data->last_value);

    dev_info(&client->dev,
        "I2C complete driver removed\n");
}

//-------------------------------------------

static const struct of_device_id i2c_complete_match[] = {
    {
        .compatible = "ldd,i2c-complete",
    },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_complete_match);

//-------------------------------------------

static struct i2c_driver i2c_complete_driver = {
    .driver = {
        .name = "ldd_i2c_complete",
        .of_match_table = i2c_complete_match,
    },

    .probe = i2c_complete_probe,
    .remove = i2c_complete_remove,
};

//-------------------------------------------

module_i2c_driver(i2c_complete_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete I2C driver example");



