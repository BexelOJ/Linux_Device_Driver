#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>

/*
 * Basic SPI driver.
 *
 * Device Tree:
 *
 * compatible = "ldd,spi-driver";
 */

 //-------------------------------------------

static int spi_driver_probe(struct spi_device* spi)
{
    pr_info("spiDriver: device matched\n");

    pr_info("spiDriver: name = %s\n", spi->modalias);
    pr_info("spiDriver: chip select = %u\n", spi->chip_select);
    pr_info("spiDriver: max speed = %u Hz\n", spi->max_speed_hz);

    return 0;
}

//-------------------------------------------

static void spi_driver_remove(struct spi_device* spi)
{
    pr_info("spiDriver: device removed\n");
}

//-------------------------------------------

static const struct of_device_id spi_driver_of_match[] =
{
    {
        .compatible = "ldd,spi-driver",
    },
    { }
};

MODULE_DEVICE_TABLE(of, spi_driver_of_match);

//-------------------------------------------

static struct spi_driver spi_driver =
{
    .driver =
    {
        .name = "ldd_spi_driver",
        .of_match_table = spi_driver_of_match,
    },

    .probe = spi_driver_probe,
    .remove = spi_driver_remove,
};

//-------------------------------------------

module_spi_driver(spi_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic SPI driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


