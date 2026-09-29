#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>

/*
 * Generic SPI register access demonstration.
 *
 * Register protocol used here:
 *
 * Write:
 *     [register][value]
 *
 * Read:
 *     [register] followed by received value
 *
 * This protocol is only an example.
 * Actual SPI devices define their own protocol.
 */

 //-------------------------------------------

static int spi_register_write(struct spi_device* spi,
    u8 reg,
    u8 value)
{
    u8 tx[2];

    tx[0] = reg;
    tx[1] = value;

    return spi_write(spi, tx, sizeof(tx));
}

//-------------------------------------------

static int spi_register_read(struct spi_device* spi,
    u8 reg,
    u8* value)
{
    int ret;

    ret = spi_write_then_read(spi,
        &reg,
        1,
        value,
        1);

    return ret;
}

//-------------------------------------------

static int spi_register_probe(struct spi_device* spi)
{
    u8 value;
    int ret;

    pr_info("spiRegister: probe()\n");

    ret = spi_register_write(spi, 0x10, 0x55);

    if (ret)
    {
        pr_err("spiRegister: write failed: %d\n", ret);
        return ret;
    }

    ret = spi_register_read(spi, 0x10, &value);

    if (ret)
    {
        pr_err("spiRegister: read failed: %d\n", ret);
        return ret;
    }

    pr_info("spiRegister: register 0x10 = 0x%02X\n", value);

    return 0;
}

//-------------------------------------------

static void spi_register_remove(struct spi_device* spi)
{
    pr_info("spiRegister: remove()\n");
}

//-------------------------------------------

static const struct of_device_id spi_register_of_match[] =
{
    {.compatible = "ldd,spi-register" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_register_of_match);

//-------------------------------------------

static struct spi_driver spi_register_driver =
{
    .driver =
    {
        .name = "ldd_spi_register",
        .of_match_table = spi_register_of_match,
    },

    .probe = spi_register_probe,
    .remove = spi_register_remove,
};

//-------------------------------------------

module_spi_driver(spi_register_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("SPI register read/write demonstration");



