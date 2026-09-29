#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>

/*
 * struct spi_device represents one SPI slave device.
 *
 * Important members:
 *
 * spi->chip_select
 * spi->max_speed_hz
 * spi->mode
 * spi->bits_per_word
 * spi->controller
 * spi->dev
 */

 //-------------------------------------------

static int spi_device_probe(struct spi_device* spi)
{
    pr_info("spiDevice: probe()\n");

    pr_info("spiDevice: chip select = %u\n",
        spi->chip_select);

    pr_info("spiDevice: max speed = %u Hz\n",
        spi->max_speed_hz);

    pr_info("spiDevice: mode = %u\n",
        spi->mode);

    pr_info("spiDevice: bits per word = %u\n",
        spi->bits_per_word);

    if (spi->controller)
    {
        pr_info("spiDevice: controller = %s\n",
            dev_name(&spi->controller->dev));
    }

    pr_info("spiDevice: device name = %s\n",
        dev_name(&spi->dev));

    return 0;
}

//-------------------------------------------

static void spi_device_remove(struct spi_device* spi)
{
    pr_info("spiDevice: remove()\n");
}

//-------------------------------------------

static const struct of_device_id spi_device_of_match[] =
{
    {.compatible = "ldd,spi-device" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_device_of_match);

//-------------------------------------------

static struct spi_driver spi_device_driver =
{
    .driver =
    {
        .name = "ldd_spi_device",
        .of_match_table = spi_device_of_match,
    },

    .probe = spi_device_probe,
    .remove = spi_device_remove,
};

//-------------------------------------------

module_spi_driver(spi_device_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("SPI device structure demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


