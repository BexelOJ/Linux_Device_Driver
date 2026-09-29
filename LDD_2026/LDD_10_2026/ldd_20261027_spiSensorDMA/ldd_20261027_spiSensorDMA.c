#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/dmaengine.h>
#include <linux/of.h>

struct spi_sensor_dma {
    struct dma_chan* rxChannel;
    struct dma_chan* txChannel;
};

//-------------------------------------------
// Probe
//-------------------------------------------

static int spi_sensor_dma_probe(struct spi_device* spi)
{
    struct spi_sensor_dma* data;
    int ret;

    data = devm_kzalloc(&spi->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->rxChannel = dma_request_chan(&spi->dev, "rx");

    if (IS_ERR(data->rxChannel)) {
        ret = PTR_ERR(data->rxChannel);

        if (ret == -EPROBE_DEFER)
            return ret;

        dev_warn(&spi->dev,
            "RX DMA unavailable: %d\n",
            ret);

        data->rxChannel = NULL;
    }

    data->txChannel = dma_request_chan(&spi->dev, "tx");

    if (IS_ERR(data->txChannel)) {
        ret = PTR_ERR(data->txChannel);

        if (ret == -EPROBE_DEFER) {
            if (data->rxChannel)
                dma_release_channel(data->rxChannel);

            return ret;
        }

        dev_warn(&spi->dev,
            "TX DMA unavailable: %d\n",
            ret);

        data->txChannel = NULL;
    }

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi->max_speed_hz = 8000000;

    ret = spi_setup(spi);

    if (ret) {
        if (data->rxChannel)
            dma_release_channel(data->rxChannel);

        if (data->txChannel)
            dma_release_channel(data->txChannel);

        return ret;
    }

    spi_set_drvdata(spi, data);

    dev_info(&spi->dev,
        "SPI sensor DMA driver loaded\n");

    if (data->rxChannel)
        dev_info(&spi->dev,
            "RX DMA channel acquired\n");

    if (data->txChannel)
        dev_info(&spi->dev,
            "TX DMA channel acquired\n");

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void spi_sensor_dma_remove(struct spi_device* spi)
{
    struct spi_sensor_dma* data;

    data = spi_get_drvdata(spi);

    if (data->rxChannel)
        dma_release_channel(data->rxChannel);

    if (data->txChannel)
        dma_release_channel(data->txChannel);

    dev_info(&spi->dev,
        "SPI sensor DMA driver removed\n");
}

//-------------------------------------------
// Device Tree
//-------------------------------------------

static const struct of_device_id spi_sensor_dma_of_match[] = {
    {.compatible = "bexel,spi-sensor-dma" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_sensor_dma_of_match);

//-------------------------------------------
// SPI driver
//-------------------------------------------

static struct spi_driver spi_sensor_dma_driver = {
    .driver = {
        .name = "spi-sensor-dma",
        .of_match_table = spi_sensor_dma_of_match,
    },

    .probe = spi_sensor_dma_probe,
    .remove = spi_sensor_dma_remove,
};

module_spi_driver(spi_sensor_dma_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("SPI sensor DMA driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


