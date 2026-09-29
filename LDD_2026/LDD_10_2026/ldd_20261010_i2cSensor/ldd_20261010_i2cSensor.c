#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>

//-------------------------------------------

#define SENSOR_REG_ID       0x00
#define SENSOR_REG_VALUE    0x01

//-------------------------------------------

static int sensor_read_reg(struct i2c_client* client,
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

static int sensor_probe(struct i2c_client* client)
{
    u8 sensor_id;
    u8 sensor_value;
    int ret;

    //-------------------------------------------
    // Read sensor ID
    //-------------------------------------------

    ret = sensor_read_reg(client,
        SENSOR_REG_ID,
        &sensor_id);

    if (ret < 0) {
        dev_err(&client->dev,
            "Failed to read sensor ID\n");

        return ret;
    }

    //-------------------------------------------
    // Read sensor value
    //-------------------------------------------

    ret = sensor_read_reg(client,
        SENSOR_REG_VALUE,
        &sensor_value);

    if (ret < 0) {
        dev_err(&client->dev,
            "Failed to read sensor value\n");

        return ret;
    }

    dev_info(&client->dev,
        "Sensor ID    = 0x%02x\n",
        sensor_id);

    dev_info(&client->dev,
        "Sensor value = %u\n",
        sensor_value);

    return 0;
}

//-------------------------------------------

static void sensor_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C sensor removed\n");
}

//-------------------------------------------

static const struct of_device_id sensor_match[] = {
    {
        .compatible = "ldd,i2c-sensor",
    },
    { }
};

MODULE_DEVICE_TABLE(of, sensor_match);

//-------------------------------------------

static struct i2c_driver sensor_driver = {
    .driver = {
        .name = "ldd_i2c_sensor",
        .of_match_table = sensor_match,
    },

    .probe = sensor_probe,
    .remove = sensor_remove,
};

//-------------------------------------------

module_i2c_driver(sensor_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Generic I2C sensor example");


/*
//-------------------------------------------



//-------------------------------------------
*/


