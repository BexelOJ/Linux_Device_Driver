#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/iio/iio.h>

/*
 * Basic IIO device.
 *
 * Demonstrates:
 *
 *     struct iio_dev
 *     struct iio_chan_spec
 *     struct iio_info
 *     read_raw()
 */

 //-------------------------------------------

static const struct iio_chan_spec iio_basic_channels[] =
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

static int iio_basic_read_raw(
    struct iio_dev* indio_dev,
    struct iio_chan_spec const* chan,
    int* val,
    int* val2,
    long mask)
{
    if (mask != IIO_CHAN_INFO_RAW)
        return -EINVAL;

    /*
     * Demonstration value.
     */

    *val = 1024;

    return IIO_VAL_INT;
}

//-------------------------------------------

static const struct iio_info iio_basic_info =
{
    .read_raw = iio_basic_read_raw,
};

//-------------------------------------------

static int iio_basic_probe(struct platform_device* pdev)
{
    struct iio_dev* indio_dev;

    pr_info("iioBasic: probe()\n");

    indio_dev = devm_iio_device_alloc(&pdev->dev,
        0);

    if (!indio_dev)
        return -ENOMEM;

    indio_dev->name = "ldd_iio_basic";

    indio_dev->info = &iio_basic_info;

    indio_dev->modes = INDIO_DIRECT_MODE;

    indio_dev->channels = iio_basic_channels;

    indio_dev->num_channels =
        ARRAY_SIZE(iio_basic_channels);

    platform_set_drvdata(pdev, indio_dev);

    return devm_iio_device_register(&pdev->dev,
        indio_dev);
}

//-------------------------------------------

static void iio_basic_remove(struct platform_device* pdev)
{
    pr_info("iioBasic: remove()\n");
}

//-------------------------------------------

static const struct of_device_id iio_basic_of_match[] =
{
    {
        .compatible = "ldd,iio-basic",
    },
    { }
};

MODULE_DEVICE_TABLE(of, iio_basic_of_match);

//-------------------------------------------

static struct platform_driver iio_basic_driver =
{
    .probe = iio_basic_probe,
    .remove = iio_basic_remove,

    .driver =
    {
        .name = "ldd_iio_basic",
        .of_match_table = iio_basic_of_match,
    },
};

//-------------------------------------------

module_platform_driver(iio_basic_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic IIO device");



