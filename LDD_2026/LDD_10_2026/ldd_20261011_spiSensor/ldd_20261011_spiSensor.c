#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/kernel.h>

/*
 * Generic SPI sensor demonstration.
 *
 * Hypothetical register map:
 *
 * 0x00 -> Device ID
 * 0x01 -> Sensor value
 */

 //-------------------------------------------

#define SENSOR_REG_ID      0x00
#define SENSOR_REG_VALUE   0x01

//-------------------------------------------

static int spi_sensor_read_reg(struct spi_device* spi,
    u8 reg,
    u8* value)
{
    return spi_write_then_read(spi,
        &reg,
        1,
        value,
        1);
}

//-------------------------------------------

static int spi_sensor_probe(struct spi_device* spi)
{
    u8 device_id;
    u8 sensor_value;

    int ret;

    pr_info("spiSensor: probe()\n");

    ret = spi_sensor_read_reg(spi,
        SENSOR_REG_ID,
        &device_id);

    if (ret)
    {
        pr_err("spiSensor: ID read failed: %d\n", ret);
        return ret;
    }

    pr_info("spiSensor: device ID = 0x%02X\n",
        device_id);

    ret = spi_sensor_read_reg(spi,
        SENSOR_REG_VALUE,
        &sensor_value);

    if (ret)
    {
        pr_err("spiSensor: sensor read failed: %d\n",
            ret);
        return ret;
    }

    pr_info("spiSensor: value = 0x%02X\n",
        sensor_value);

    return 0;
}

//-------------------------------------------

static void spi_sensor_remove(struct spi_device* spi)
{
    pr_info("spiSensor: remove()\n");
}

//-------------------------------------------

static const struct of_device_id spi_sensor_of_match[] =
{
    {.compatible = "ldd,spi-sensor" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_sensor_of_match);

//-------------------------------------------

static struct spi_driver spi_sensor_driver =
{
    .driver =
    {
        .name = "ldd_spi_sensor",
        .of_match_table = spi_sensor_of_match,
    },

    .probe = spi_sensor_probe,
    .remove = spi_sensor_remove,
};

//-------------------------------------------

module_spi_driver(spi_sensor_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Generic SPI sensor driver");



