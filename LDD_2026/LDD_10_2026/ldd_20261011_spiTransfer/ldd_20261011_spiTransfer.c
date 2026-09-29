#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>

/*
 * Basic SPI transfer demonstration.
 *
 * This example sends 4 bytes and receives 4 bytes.
 * The actual response depends on the connected SPI device.
 */

 //-------------------------------------------

static int spi_transfer_probe(struct spi_device* spi)
{
    struct spi_transfer transfer = { 0 };
    u8 tx_buffer[4] = { 0xAA, 0x55, 0x12, 0x34 };
    u8 rx_buffer[4] = { 0 };

    int ret;

    pr_info("spiTransfer: probe()\n");

    transfer.tx_buf = tx_buffer;
    transfer.rx_buf = rx_buffer;
    transfer.len = sizeof(tx_buffer);

    ret = spi_sync_transfer(spi, &transfer, 1);

    if (ret < 0)
    {
        pr_err("spiTransfer: transfer failed: %d\n", ret);
        return ret;
    }

    pr_info("spiTransfer: RX = %02X %02X %02X %02X\n",
        rx_buffer[0],
        rx_buffer[1],
        rx_buffer[2],
        rx_buffer[3]);

    return 0;
}

//-------------------------------------------

static void spi_transfer_remove(struct spi_device* spi)
{
    pr_info("spiTransfer: remove()\n");
}

//-------------------------------------------

static const struct of_device_id spi_transfer_of_match[] =
{
    {.compatible = "ldd,spi-transfer" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_transfer_of_match);

//-------------------------------------------

static struct spi_driver spi_transfer_driver =
{
    .driver =
    {
        .name = "ldd_spi_transfer",
        .of_match_table = spi_transfer_of_match,
    },

    .probe = spi_transfer_probe,
    .remove = spi_transfer_remove,
};

//-------------------------------------------

module_spi_driver(spi_transfer_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic SPI transfer demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


