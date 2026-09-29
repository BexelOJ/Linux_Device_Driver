#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/dmaengine.h>
#include <linux/of.h>

struct i2c_sensor_dma {
    struct dma_chan* rxChannel;
    struct dma_chan* txChannel;
};

//-------------------------------------------
// Probe
//-------------------------------------------

static int i2c_sensor_dma_probe(struct i2c_client* client)
{
    struct i2c_sensor_dma* data;
    int ret;

    data = devm_kzalloc(&client->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->rxChannel = dma_request_chan(&client->dev, "rx");

    if (IS_ERR(data->rxChannel)) {
        ret = PTR_ERR(data->rxChannel);

        if (ret == -EPROBE_DEFER)
            return ret;

        dev_warn(&client->dev,
            "RX DMA unavailable: %d\n", ret);

        data->rxChannel = NULL;
    }

    data->txChannel = dma_request_chan(&client->dev, "tx");

    if (IS_ERR(data->txChannel)) {
        ret = PTR_ERR(data->txChannel);

        if (ret == -EPROBE_DEFER) {
            if (data->rxChannel)
                dma_release_channel(data->rxChannel);

            return ret;
        }

        dev_warn(&client->dev,
            "TX DMA unavailable: %d\n", ret);

        data->txChannel = NULL;
    }

    i2c_set_clientdata(client, data);

    dev_info(&client->dev,
        "I2C sensor DMA driver probed\n");

    if (data->rxChannel)
        dev_info(&client->dev, "RX DMA available\n");

    if (data->txChannel)
        dev_info(&client->dev, "TX DMA available\n");

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void i2c_sensor_dma_remove(struct i2c_client* client)
{
    struct i2c_sensor_dma* data;

    data = i2c_get_clientdata(client);

    if (data->rxChannel)
        dma_release_channel(data->rxChannel);

    if (data->txChannel)
        dma_release_channel(data->txChannel);

    dev_info(&client->dev,
        "I2C sensor DMA driver removed\n");
}

//-------------------------------------------
// Device Tree
//-------------------------------------------

static const struct of_device_id i2c_sensor_dma_of_match[] = {
    {.compatible = "bexel,i2c-sensor-dma" },
    { }
};

MODULE_DEVICE_TABLE(of, i2c_sensor_dma_of_match);

//-------------------------------------------
// I2C driver
//-------------------------------------------

static struct i2c_driver i2c_sensor_dma_driver = {
    .driver = {
        .name = "i2c-sensor-dma",
        .of_match_table = i2c_sensor_dma_of_match,
    },

    .probe = i2c_sensor_dma_probe,
    .remove = i2c_sensor_dma_remove,
};

module_i2c_driver(i2c_sensor_dma_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("I2C sensor DMA driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


