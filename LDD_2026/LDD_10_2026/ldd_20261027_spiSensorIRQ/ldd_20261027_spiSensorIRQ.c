#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/interrupt.h>
#include <linux/of.h>

struct spi_sensor_irq {
    struct spi_device* spi;
    int irq;
};

//-------------------------------------------
// Interrupt handler
//-------------------------------------------

static irqreturn_t spi_sensor_irq_handler(int irq,
    void* dev_id)
{
    struct spi_sensor_irq* data = dev_id;

    pr_info("spi_sensor_irq: sensor interrupt\n");

    dev_info(&data->spi->dev,
        "SPI sensor interrupt received\n");

    return IRQ_HANDLED;
}

//-------------------------------------------
// Probe
//-------------------------------------------

static int spi_sensor_irq_probe(struct spi_device* spi)
{
    struct spi_sensor_irq* data;
    int ret;

    data = devm_kzalloc(&spi->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->spi = spi;
    data->irq = spi->irq;

    if (spi->irq <= 0) {
        dev_err(&spi->dev,
            "SPI sensor IRQ unavailable\n");
        return -EINVAL;
    }

    spi->mode = SPI_MODE_0;
    spi->bits_per_word = 8;
    spi->max_speed_hz = 8000000;

    ret = spi_setup(spi);

    if (ret)
        return ret;

    ret = devm_request_threaded_irq(&spi->dev,
        spi->irq,
        NULL,
        spi_sensor_irq_handler,
        IRQF_ONESHOT |
        IRQF_TRIGGER_RISING,
        "spi_sensor_irq",
        data);

    if (ret)
        return ret;

    spi_set_drvdata(spi, data);

    dev_info(&spi->dev,
        "SPI sensor IRQ driver loaded\n");

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void spi_sensor_irq_remove(struct spi_device* spi)
{
    dev_info(&spi->dev,
        "SPI sensor IRQ driver removed\n");
}

//-------------------------------------------
// Device Tree
//-------------------------------------------

static const struct of_device_id spi_sensor_irq_of_match[] = {
    {.compatible = "bexel,spi-sensor-irq" },
    { }
};

MODULE_DEVICE_TABLE(of, spi_sensor_irq_of_match);

//-------------------------------------------
// SPI driver
//-------------------------------------------

static struct spi_driver spi_sensor_irq_driver = {
    .driver = {
        .name = "spi-sensor-irq",
        .of_match_table = spi_sensor_irq_of_match,
    },

    .probe = spi_sensor_irq_probe,
    .remove = spi_sensor_irq_remove,
};

module_spi_driver(spi_sensor_irq_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("SPI sensor IRQ driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


