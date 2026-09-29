#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mutex.h>

struct production_sensor {
    struct mutex lock;
    int value;
    bool initialized;
};

//-------------------------------------------
// Read sensor
//-------------------------------------------

static int production_sensor_read(struct production_sensor* sensor)
{
    int value;

    mutex_lock(&sensor->lock);

    value = sensor->value;

    mutex_unlock(&sensor->lock);

    return value;
}

//-------------------------------------------
// Write sensor
//-------------------------------------------

static void production_sensor_write(struct production_sensor* sensor,
    int value)
{
    mutex_lock(&sensor->lock);

    sensor->value = value;

    mutex_unlock(&sensor->lock);
}

//-------------------------------------------
// Probe
//-------------------------------------------

static int production_probe(struct platform_device* pdev)
{
    struct production_sensor* sensor;

    sensor = devm_kzalloc(&pdev->dev,
        sizeof(*sensor),
        GFP_KERNEL);

    if (!sensor)
        return -ENOMEM;

    mutex_init(&sensor->lock);

    production_sensor_write(sensor, 2500);

    sensor->initialized = true;

    platform_set_drvdata(pdev, sensor);

    dev_info(&pdev->dev,
        "production driver initialized\n");

    dev_info(&pdev->dev,
        "initial sensor value = %d\n",
        production_sensor_read(sensor));

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void production_remove(struct platform_device* pdev)
{
    struct production_sensor* sensor;

    sensor = platform_get_drvdata(pdev);

    sensor->initialized = false;

    dev_info(&pdev->dev,
        "production driver removed\n");
}

//-------------------------------------------
// Device Tree
//-------------------------------------------

static const struct of_device_id production_of_match[] = {
    {.compatible = "bexel,production-sensor" },
    { }
};

MODULE_DEVICE_TABLE(of, production_of_match);

//-------------------------------------------
// Platform driver
//-------------------------------------------

static struct platform_driver production_driver = {
    .probe = production_probe,
    .remove = production_remove,

    .driver = {
        .name = "production-sensor",
        .of_match_table = production_of_match,
    },
};

module_platform_driver(production_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Production-style embedded sensor driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


