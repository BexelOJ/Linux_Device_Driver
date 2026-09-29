//-------------------------------------------
// ldd_20261006_hardwareIRQDriver.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>

//-------------------------------------------
// IRQ Handler
//-------------------------------------------

static irqreturn_t hardware_irq_handler(
    int irq,
    void* dev_id)
{
    struct device* dev = dev_id;

    dev_info(
        dev,
        "Hardware interrupt received\n"
    );

    return IRQ_HANDLED;
}

//-------------------------------------------
// Probe
//-------------------------------------------

static int hardware_irq_probe(
    struct platform_device* pdev)
{
    int irq;
    int ret;

    dev_info(
        &pdev->dev,
        "hardwareIRQDriver: probe\n"
    );

    //-------------------------------------------
    // Get IRQ from Device Tree / ACPI
    //-------------------------------------------

    irq = platform_get_irq(
        pdev,
        0
    );

    if (irq < 0)
    {
        dev_err(
            &pdev->dev,
            "No IRQ found\n"
        );

        return irq;
    }

    dev_info(
        &pdev->dev,
        "Using IRQ %d\n",
        irq
    );

    //-------------------------------------------
    // Request IRQ
    //-------------------------------------------

    ret = devm_request_irq(
        &pdev->dev,
        irq,
        hardware_irq_handler,
        0,
        "ldd_hardware_irq",
        &pdev->dev
    );

    if (ret)
    {
        dev_err(
            &pdev->dev,
            "Failed to request IRQ\n"
        );

        return ret;
    }

    dev_info(
        &pdev->dev,
        "IRQ registered successfully\n"
    );

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void hardware_irq_remove(
    struct platform_device* pdev)
{
    dev_info(
        &pdev->dev,
        "hardwareIRQDriver: remove\n"
    );

    /*
     * No explicit free_irq() needed.
     *
     * devm_request_irq() automatically releases
     * the IRQ when the device is removed.
     */
}

//-------------------------------------------
// Platform Driver
//-------------------------------------------

static struct platform_driver hardware_irq_driver =
{
    .probe = hardware_irq_probe,
    .remove = hardware_irq_remove,

    .driver =
    {
        .name = "ldd_hardware_irq",
    },
};

//-------------------------------------------

module_platform_driver(
    hardware_irq_driver
);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION(
    "Basic hardware IRQ platform driver"
);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/




