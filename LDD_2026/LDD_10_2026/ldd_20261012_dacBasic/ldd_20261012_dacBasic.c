#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/iio/consumer.h>

/*
 * Basic DAC consumer.
 *
 * The DAC itself is normally implemented as
 * an IIO provider.
 */

 //-------------------------------------------

struct dac_basic_data
{
    struct iio_channel* channel;
};

//-------------------------------------------

static int dac_basic_probe(struct platform_device* pdev)
{
    struct dac_basic_data* data;
    int value;
    int ret;

    pr_info("dacBasic: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->channel =
        devm_iio_channel_get(&pdev->dev, NULL);

    if (IS_ERR(data->channel))
    {
        ret = PTR_ERR(data->channel);

        pr_err("dacBasic: channel get failed: %d\n",
            ret);

        return ret;
    }

    /*
     * Example digital output value.
     *
     * Actual DAC scaling depends on the DAC hardware.
     */

    value = 2048;

    ret = iio_write_channel_raw(data->channel,
        value);

    if (ret)
    {
        pr_err("dacBasic: DAC write failed: %d\n",
            ret);

        return ret;
    }

    pr_info("dacBasic: raw value = %d\n",
        value);

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void dac_basic_remove(struct platform_device* pdev)
{
    pr_info("dacBasic: remove()\n");
}

//-------------------------------------------

static const struct of_device_id dac_basic_of_match[] =
{
    {
        .compatible = "ldd,dac-basic",
    },
    { }
};

MODULE_DEVICE_TABLE(of, dac_basic_of_match);

//-------------------------------------------

static struct platform_driver dac_basic_driver =
{
    .probe = dac_basic_probe,
    .remove = dac_basic_remove,

    .driver =
    {
        .name = "ldd_dac_basic",
        .of_match_table = dac_basic_of_match,
    },
};

//-------------------------------------------

module_platform_driver(dac_basic_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic DAC consumer using IIO");



