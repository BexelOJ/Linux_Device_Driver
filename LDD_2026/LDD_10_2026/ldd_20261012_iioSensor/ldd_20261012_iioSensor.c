#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/iio/iio.h>

/*
 * Simple IIO sensor.
 *
 * This example exposes one raw channel.
 */

 //-------------------------------------------

static const struct iio_chan_spec iio_sensor_channels[] =
{
    {
        .type = IIO_VOLTAGE,
        .indexed = 1,
        .channel = 0,
        .info_mask_separate =
            BIT(IIO_CHAN_INFO_RAW),
    },
};

//-------------------------------------------

static int iio_sensor_read_raw(struct iio_dev* indio_dev,
    struct iio_chan_spec const* chan,
    int* val,
    int* val2,
    long mask)
{
    switch (mask)
    {
    case IIO_CHAN_INFO_RAW:

        /*
         * Example raw value.
         */

        *val = 2048;

        return IIO_VAL_INT;

    default:

        return -EINVAL;
    }
}

//-------------------------------------------

static const struct iio_info iio_sensor_info =
{
    .read_raw = iio_sensor_read_raw,
};

//-------------------------------------------

static int iio_sensor_probe(struct platform_device* pdev)
{
    struct iio_dev* indio_dev;

    indio_dev = devm_iio_device_alloc(&pdev->dev,
        0);

    if (!indio_dev)
        return -ENOMEM;

    indio_dev->name = "ldd_iio_sensor";
    indio_dev->info = &iio_sensor_info;
    indio_dev->modes = INDIO_DIRECT_MODE;

    indio_dev->channels = iio_sensor_channels;
    indio_dev->num_channels =
        ARRAY_SIZE(iio_sensor_channels);

    platform_set_drvdata(pdev, indio_dev);

    pr_info("iioSensor: registering IIO device\n");

    return devm_iio_device_register(&pdev->dev,
        indio_dev);
}

//-------------------------------------------

static void iio_sensor_remove(struct platform_device* pdev)
{
    pr_info("iioSensor: remove()\n");
}

//-------------------------------------------

static const struct of_device_id iio_sensor_of_match[] =
{
    {
        .compatible = "ldd,iio-sensor",
    },
    { }
};

MODULE_DEVICE_TABLE(of, iio_sensor_of_match);

//-------------------------------------------

static struct platform_driver iio_sensor_driver =
{
    .probe = iio_sensor_probe,
    .remove = iio_sensor_remove,

    .driver =
    {
        .name = "ldd_iio_sensor",
        .of_match_table = iio_sensor_of_match,
    },
};

//-------------------------------------------

module_platform_driver(iio_sensor_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic IIO sensor provider");



