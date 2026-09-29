#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spi/spi.h>

#define DRIVER_NAME "ldd_20261026_rpiSPIDisplay"

struct rpi_spi_display {
    struct spi_device* spi;
};

static int rpiSPIDisplay_probe(struct spi_device* spi)
{
    struct rpi_spi_display* data;
    int ret;

    data = devm_kzalloc(&spi->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->spi = spi;

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi->max_speed_hz = 8000000;

    ret = spi_setup(spi);

    if (ret)
        return ret;

    spi_set_drvdata(spi, data);

    dev_info(&spi->dev,
        "%s: SPI display initialized\n",
        DRIVER_NAME);

    dev_info(&spi->dev,
        "bus=%d cs=%u speed=%u\n",
        spi->controller->bus_num,
        spi->chip_select,
        spi->max_speed_hz);

    return 0;
}

static void rpiSPIDisplay_remove(struct spi_device* spi)
{
    dev_info(&spi->dev,
        "%s: SPI display removed\n",
        DRIVER_NAME);
}

static const struct of_device_id rpiSPIDisplay_of_match[] = {
    {
        .compatible = "bexel,rpi-spi-display",
    },
    { }
};

MODULE_DEVICE_TABLE(of, rpiSPIDisplay_of_match);

static struct spi_driver rpiSPIDisplay_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = rpiSPIDisplay_of_match,
    },

    .probe = rpiSPIDisplay_probe,
    .remove = rpiSPIDisplay_remove,
};

module_spi_driver(rpiSPIDisplay_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Raspberry Pi SPI display driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


