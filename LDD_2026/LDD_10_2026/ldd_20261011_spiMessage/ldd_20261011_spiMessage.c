#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>

/*
 * SPI message demonstration.
 *
 * One spi_message can contain multiple spi_transfer objects.
 */

 //-------------------------------------------

static int spi_message_probe(struct spi_device* spi)
{
    struct spi_message message;

    struct spi_transfer transfer1 = { 0 };
    struct spi_transfer transfer2 = { 0 };

    u8 command = 0x9A;
    u8 tx_data[2] = { 0x12, 0x34 };
    u8 rx_data[2] = { 0 };

    int ret;

    pr_info("spiMessage: probe()\n");

    spi_message_init(&message);

    //---------------------------------------
    // Transfer 1: command
    //---------------------------------------

    transfer1.tx_buf = &command;
    transfer1.len = 1;

    spi_message_add_tail(&transfer1, &message);

    //---------------------------------------
    // Transfer 2: data
    //---------------------------------------

    transfer2.tx_buf = tx_data;
    transfer2.rx_buf = rx_data;
    transfer2.len = sizeof(tx_data);

    spi_message_add_tail(&transfer2, &message);

    //---------------------------------------
    // Execute complete message
    //---------------------------------------

    ret = spi_sync(spi, &message);

    if (ret)
    {
        pr_err("spiMessage: spi_sync() failed: %d\n",
            ret);

        return ret;
    }

    pr_info("spiMessage: RX = %02X %02X\n",
        rx_data[0],
        rx_data[1]);

    return 0;
}

//-------------------------------------------

static void spi_message_remove(struct spi_device* spi)
{
    pr_info("spiMessage: remove()\n");
}

//-------------------------------------------

static const struct of_device_id spi_message_of_match[] =
{
    {.compatible = "ldd,spi-message" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_message_of_match);

//-------------------------------------------

static struct spi_driver spi_message_driver =
{
    .driver =
    {
        .name = "ldd_spi_message",
        .of_match_table = spi_message_of_match,
    },

    .probe = spi_message_probe,
    .remove = spi_message_remove,
};

//-------------------------------------------

module_spi_driver(spi_message_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("SPI message demonstration");



