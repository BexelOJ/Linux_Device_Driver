#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/of.h>

struct i2c_sensor_irq {
    int irq;
};

//-------------------------------------------
// Interrupt handler
//-------------------------------------------

static irqreturn_t i2c_sensor_irq_handler(int irq,
    void* dev_id)
{
    struct i2c_sensor_irq* data = dev_id;

    dev_info(NULL,
        "I2C sensor interrupt received\n");

    data->irq = irq;

    return IRQ_HANDLED;
}

//-------------------------------------------
// Probe
//-------------------------------------------

static int i2c_sensor_irq_probe(struct i2c_client* client)
{
    struct i2c_sensor_irq* data;
    int ret;

    data = devm_kzalloc(&client->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->irq = client->irq;

    if (client->irq <= 0) {
        dev_err(&client->dev,
            "IRQ not available\n");
        return -EINVAL;
    }

    ret = devm_request_threaded_irq(&client->dev,
        client->irq,
        NULL,
        i2c_sensor_irq_handler,
        IRQF_ONESHOT |
        IRQF_TRIGGER_RISING,
        "i2c_sensor_irq",
        data);

    if (ret) {
        dev_err(&client->dev,
            "failed to request IRQ: %d\n",
            ret);
        return ret;
    }

    i2c_set_clientdata(client, data);

    dev_info(&client->dev,
        "I2C sensor IRQ driver probed\n");

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void i2c_sensor_irq_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "I2C sensor IRQ driver removed\n");
}

//-------------------------------------------
// Device Tree
//-------------------------------------------

static const struct of_device_id i2c_sensor_irq_of_match[] = {
    {.compatible = "bexel,i2c-sensor-irq" },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_sensor_irq_of_match);

//-------------------------------------------
// I2C driver
//-------------------------------------------

static struct i2c_driver i2c_sensor_irq_driver = {
    .driver = {
        .name = "i2c-sensor-irq",
        .of_match_table = i2c_sensor_irq_of_match,
    },

    .probe = i2c_sensor_irq_probe,
    .remove = i2c_sensor_irq_remove,
};

module_i2c_driver(i2c_sensor_irq_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("I2C sensor IRQ driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


