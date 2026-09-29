#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>

/*
 * SPI mode demonstration.
 *
 * Mode 0:
 * CPOL = 0
 * CPHA = 0
 */

 //-------------------------------------------

static int spi_mode_probe(struct spi_device* spi)
{
    int ret;

    pr_info("spiMode: probe()\n");

    spi->mode = SPI_MODE_0;

    ret = spi_setup(spi);

    if (ret)
    {
        pr_err("spiMode: spi_setup() failed: %d\n", ret);
        return ret;
    }

    pr_info("spiMode: mode = %u\n", spi->mode);
    pr_info("spiMode: max_speed_hz = %u\n", spi->max_speed_hz);
    pr_info("spiMode: bits_per_word = %u\n", spi->bits_per_word);

    return 0;
}

//-------------------------------------------

static void spi_mode_remove(struct spi_device* spi)
{
    pr_info("spiMode: remove()\n");
}

//-------------------------------------------

static const struct of_device_id spi_mode_of_match[] =
{
    {.compatible = "ldd,spi-mode" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_mode_of_match);

//-------------------------------------------

static struct spi_driver spi_mode_driver =
{
    .driver =
    {
        .name = "ldd_spi_mode",
        .of_match_table = spi_mode_of_match,
    },

    .probe = spi_mode_probe,
    .remove = spi_mode_remove,
};

//-------------------------------------------

module_spi_driver(spi_mode_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("SPI mode configuration demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


