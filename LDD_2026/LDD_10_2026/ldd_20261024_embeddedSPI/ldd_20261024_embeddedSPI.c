#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spi/spi.h>

#define DRIVER_NAME "ldd_20261024_embeddedSPI"

static int embeddedSPI_probe(struct spi_device* spi)
{
    pr_info("%s: SPI device detected\n",
        DRIVER_NAME);

    pr_info("%s: bus=%d chip_select=%u\n",
        DRIVER_NAME,
        spi->controller->bus_num,
        spi->chip_select);

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;

    return spi_setup(spi);
}

static void embeddedSPI_remove(struct spi_device* spi)
{
    pr_info("%s: SPI device removed\n",
        DRIVER_NAME);
}

static const struct of_device_id embeddedSPI_of_match[] = {
    {
        .compatible = "bexel,embedded-spi",
    },
    { }
};

MODULE_DEVICE_TABLE(of, embeddedSPI_of_match);

static struct spi_driver embeddedSPI_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = embeddedSPI_of_match,
    },
    .probe = embeddedSPI_probe,
    .remove = embeddedSPI_remove,
};

module_spi_driver(embeddedSPI_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Embedded Linux SPI driver example");



