#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>

/*
 * Generic SPI display demonstration.
 *
 * Real displays may require:
 *
 * DC GPIO
 * RESET GPIO
 * specific initialization sequence
 * specific command/data protocol
 */

 //-------------------------------------------

static int spi_display_send_command(struct spi_device* spi, u8 command)
{
    u8 tx = command;

    return spi_write(spi, &tx, 1);
}

//-------------------------------------------

static int spi_display_send_data(struct spi_device* spi,
    const u8* data,
    size_t length)
{
    return spi_write(spi, data, length);
}

//-------------------------------------------

static int spi_display_probe(struct spi_device* spi)
{
    u8 data[] = { 0xAA, 0x55, 0x12, 0x34 };

    int ret;

    pr_info("spiDisplay: probe()\n");

    ret = spi_display_send_command(spi, 0x01);

    if (ret)
    {
        pr_err("spiDisplay: command failed: %d\n", ret);
        return ret;
    }

    ret = spi_display_send_data(spi, data, sizeof(data));

    if (ret)
    {
        pr_err("spiDisplay: data transfer failed: %d\n", ret);
        return ret;
    }

    pr_info("spiDisplay: transfer complete\n");

    return 0;
}

//-------------------------------------------

static void spi_display_remove(struct spi_device* spi)
{
    pr_info("spiDisplay: remove()\n");
}

//-------------------------------------------

static const struct of_device_id spi_display_of_match[] =
{
    {.compatible = "ldd,spi-display" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_display_of_match);

//-------------------------------------------

static struct spi_driver spi_display_driver =
{
    .driver =
    {
        .name = "ldd_spi_display",
        .of_match_table = spi_display_of_match,
    },

    .probe = spi_display_probe,
    .remove = spi_display_remove,
};

//-------------------------------------------

module_spi_driver(spi_display_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Generic SPI display demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


