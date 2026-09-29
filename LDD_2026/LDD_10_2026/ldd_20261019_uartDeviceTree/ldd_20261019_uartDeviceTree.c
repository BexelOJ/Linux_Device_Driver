#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/io.h>

//-------------------------------------------

struct ldd_uart_data {
    void __iomem* base;
    resource_size_t size;
};

//-------------------------------------------

static int ldd_uart_probe(struct platform_device* pdev)
{
    struct ldd_uart_data* data;
    struct resource* res;

    dev_info(&pdev->dev,
        "ldd_uartDeviceTree: probe\n");

    //-------------------------------------------
    // Allocate driver data
    //-------------------------------------------

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Get memory resource from Device Tree
    //-------------------------------------------

    res = platform_get_resource(pdev,
        IORESOURCE_MEM,
        0);

    if (!res) {
        dev_err(&pdev->dev,
            "memory resource not found\n");

        return -ENODEV;
    }

    //-------------------------------------------
    // Map UART registers
    //-------------------------------------------

    data->base = devm_ioremap_resource(&pdev->dev,
        res);

    if (IS_ERR(data->base))
        return PTR_ERR(data->base);

    data->size = resource_size(res);

    //-------------------------------------------

    platform_set_drvdata(pdev, data);

    dev_info(&pdev->dev,
        "UART register area mapped\n");

    dev_info(&pdev->dev,
        "size = %pa\n",
        &data->size);

    return 0;
}

//-------------------------------------------

static void ldd_uart_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "ldd_uartDeviceTree: remove\n");
}

//-------------------------------------------

static const struct of_device_id
ldd_uart_of_match[] = {

    {
        .compatible = "ldd,uart",
    },

    { }
};

MODULE_DEVICE_TABLE(of, ldd_uart_of_match);

//-------------------------------------------

static struct platform_driver ldd_uart_driver = {

    .probe = ldd_uart_probe,
    .remove = ldd_uart_remove,

    .driver = {
        .name = "ldd-uart-dt",
        .of_match_table = ldd_uart_of_match,
    },
};

//-------------------------------------------

module_platform_driver(ldd_uart_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("UART Device Tree demonstration");


/*

Device Tree matching for a UART platform device:


The real flow is:

Device Tree
     │
     ▼
platform_device
     │
     ▼
probe()
     │
     ▼
platform_get_resource()
     │
     ▼
devm_ioremap_resource()
     │
     ▼
UART registers

*/


