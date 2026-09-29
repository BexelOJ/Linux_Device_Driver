#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/iio/consumer.h>

/*
 * Basic ADC consumer.
 *
 * The ADC itself is normally an IIO provider.
 * This driver acts as an IIO consumer.
 */

 //-------------------------------------------

struct adc_basic_data
{
    struct iio_channel* channel;
};

//-------------------------------------------

static int adc_basic_probe(struct platform_device* pdev)
{
    struct adc_basic_data* data;
    int value;
    int ret;

    pr_info("adcBasic: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->channel = devm_iio_channel_get(&pdev->dev, NULL);

    if (IS_ERR(data->channel))
    {
        ret = PTR_ERR(data->channel);

        pr_err("adcBasic: IIO channel failed: %d\n",
            ret);

        return ret;
    }

    ret = iio_read_channel_raw(data->channel, &value);

    if (ret)
    {
        pr_err("adcBasic: ADC read failed: %d\n",
            ret);

        return ret;
    }

    pr_info("adcBasic: raw ADC value = %d\n",
        value);

    platform_set_drvdata(pdev, data);

    return 0;
}

//-------------------------------------------

static void adc_basic_remove(struct platform_device* pdev)
{
    pr_info("adcBasic: remove()\n");
}

//-------------------------------------------

static const struct of_device_id adc_basic_of_match[] =
{
    {
        .compatible = "ldd,adc-basic",
    },
    { }
};

MODULE_DEVICE_TABLE(of, adc_basic_of_match);

//-------------------------------------------

static struct platform_driver adc_basic_driver =
{
    .probe = adc_basic_probe,
    .remove = adc_basic_remove,

    .driver =
    {
        .name = "ldd_adc_basic",
        .of_match_table = adc_basic_of_match,
    },
};

//-------------------------------------------

module_platform_driver(adc_basic_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic ADC consumer using IIO");


/*
//-------------------------------------------



//-------------------------------------------
*/


