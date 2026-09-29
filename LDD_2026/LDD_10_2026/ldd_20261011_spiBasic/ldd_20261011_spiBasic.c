#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>

/*
 * Basic SPI driver.
 */

 //-------------------------------------------

static int spi_basic_probe(struct spi_device* spi)
{
    pr_info("spiBasic: SPI device detected\n");

    pr_info("spiBasic: chip select = %u\n",
        spi->chip_select);

    pr_info("spiBasic: mode = %u\n",
        spi->mode);

    pr_info("spiBasic: speed = %u Hz\n",
        spi->max_speed_hz);

    pr_info("spiBasic: bits per word = %u\n",
        spi->bits_per_word);

    return 0;
}

//-------------------------------------------

static void spi_basic_remove(struct spi_device* spi)
{
    pr_info("spiBasic: SPI device removed\n");
}

//-------------------------------------------

static const struct of_device_id spi_basic_of_match[] =
{
    {.compatible = "ldd,spi-basic" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_basic_of_match);

//-------------------------------------------

static struct spi_driver spi_basic_driver =
{
    .driver =
    {
        .name = "ldd_spi_basic",
        .of_match_table = spi_basic_of_match,
    },

    .probe = spi_basic_probe,
    .remove = spi_basic_remove,
};

//-------------------------------------------

module_spi_driver(spi_basic_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic SPI driver demonstration");



