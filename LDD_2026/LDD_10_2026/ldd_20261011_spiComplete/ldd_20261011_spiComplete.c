#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>
#include <linux/slab.h>

/*
 * Complete SPI driver demonstration.
 */

 //-------------------------------------------

struct spi_complete_data
{
    struct spi_device* spi;

    u8 tx_buffer[4];
    u8 rx_buffer[4];
};

//-------------------------------------------

static int spi_complete_transfer(struct spi_complete_data* data)
{
    struct spi_transfer transfer = { 0 };

    int ret;

    transfer.tx_buf = data->tx_buffer;
    transfer.rx_buf = data->rx_buffer;
    transfer.len = sizeof(data->tx_buffer);

    ret = spi_sync_transfer(data->spi, &transfer, 1);

    if (ret)
        return ret;

    return 0;
}

//-------------------------------------------

static int spi_complete_probe(struct spi_device* spi)
{
    struct spi_complete_data* data;
    int ret;

    pr_info("spiComplete: probe()\n");

    data = devm_kzalloc(&spi->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->spi = spi;

    data->tx_buffer[0] = 0xAA;
    data->tx_buffer[1] = 0x55;
    data->tx_buffer[2] = 0x12;
    data->tx_buffer[3] = 0x34;

    spi_set_drvdata(spi, data);

    ret = spi_complete_transfer(data);

    if (ret)
    {
        pr_err("spiComplete: transfer failed: %d\n", ret);
        return ret;
    }

    pr_info("spiComplete: RX = %02X %02X %02X %02X\n",
        data->rx_buffer[0],
        data->rx_buffer[1],
        data->rx_buffer[2],
        data->rx_buffer[3]);

    return 0;
}

//-------------------------------------------

static void spi_complete_remove(struct spi_device* spi)
{
    struct spi_complete_data* data;

    data = spi_get_drvdata(spi);

    pr_info("spiComplete: remove()\n");

    if (data)
        pr_info("spiComplete: private data found\n");
}

//-------------------------------------------

static const struct of_device_id spi_complete_of_match[] =
{
    {.compatible = "ldd,spi-complete" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_complete_of_match);

//-------------------------------------------

static struct spi_driver spi_complete_driver =
{
    .driver =
    {
        .name = "ldd_spi_complete",
        .of_match_table = spi_complete_of_match,
    },

    .probe = spi_complete_probe,
    .remove = spi_complete_remove,
};

//-------------------------------------------

module_spi_driver(spi_complete_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete SPI driver demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


