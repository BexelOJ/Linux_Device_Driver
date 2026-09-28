#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/slab.h>

//-------------------------------------------

struct dt_driver_data {
    u32 device_id;
    int irq;
};

//-------------------------------------------

static irqreturn_t dt_complete_irq(int irq, void* data)
{
    struct dt_driver_data* drvdata = data;

    pr_info("DT complete driver IRQ: %d\n",
        drvdata->irq);

    return IRQ_HANDLED;
}

//-------------------------------------------

static int dt_complete_probe(struct platform_device* pdev)
{
    struct dt_driver_data* data;
    u32 device_id;
    int irq;
    int ret;

    dev_info(&pdev->dev,
        "probe() started\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Read DT property
    //-------------------------------------------

    ret = of_property_read_u32(pdev->dev.of_node,
        "device-id",
        &device_id);

    if (ret) {
        dev_err(&pdev->dev,
            "device-id property missing\n");

        return ret;
    }

    //-------------------------------------------
    // Get IRQ from Device Tree
    //-------------------------------------------

    irq = platform_get_irq(pdev, 0);

    if (irq < 0) {
        dev_err(&pdev->dev,
            "IRQ not found\n");

        return irq;
    }

    data->device_id = device_id;
    data->irq = irq;

    platform_set_drvdata(pdev, data);

    //-------------------------------------------
    // Request IRQ
    //-------------------------------------------

    ret = devm_request_irq(&pdev->dev,
        irq,
        dt_complete_irq,
        0,
        "ldd_dt_complete",
        data);

    if (ret) {
        dev_err(&pdev->dev,
            "request_irq() failed\n");

        return ret;
    }

    dev_info(&pdev->dev,
        "Device ID : %u\n",
        data->device_id);

    dev_info(&pdev->dev,
        "IRQ      : %d\n",
        data->irq);

    return 0;
}

//-------------------------------------------

static int dt_complete_remove(struct platform_device* pdev)
{
    struct dt_driver_data* data;

    data = platform_get_drvdata(pdev);

    if (data) {
        dev_info(&pdev->dev,
            "Removing device ID %u\n",
            data->device_id);
    }

    return 0;
}

//-------------------------------------------

static const struct of_device_id dt_complete_match[] = {
    {
        .compatible = "ldd,complete-device",
    },
    { }
};

MODULE_DEVICE_TABLE(of, dt_complete_match);

//-------------------------------------------

static struct platform_driver dt_complete_driver = {
    .probe = dt_complete_probe,
    .remove = dt_complete_remove,

    .driver = {
        .name = "ldd_dt_complete",
        .of_match_table = dt_complete_match,
    },
};

//-------------------------------------------

module_platform_driver(dt_complete_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete Device Tree platform driver");



