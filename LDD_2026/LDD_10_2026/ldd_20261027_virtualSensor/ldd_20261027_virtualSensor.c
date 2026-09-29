#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/mutex.h>

struct virtual_sensor {
    struct mutex lock;
    int temperature;
    int humidity;
};

//-------------------------------------------
// Read virtual sensor
//-------------------------------------------

static void virtual_sensor_read(struct virtual_sensor* sensor,
    int* temperature,
    int* humidity)
{
    mutex_lock(&sensor->lock);

    *temperature = sensor->temperature;
    *humidity = sensor->humidity;

    mutex_unlock(&sensor->lock);
}

//-------------------------------------------
// Update virtual sensor
//-------------------------------------------

static void virtual_sensor_update(struct virtual_sensor* sensor)
{
    mutex_lock(&sensor->lock);

    sensor->temperature += 1;

    if (sensor->temperature > 3000)
        sensor->temperature = 2500;

    sensor->humidity = 600;

    mutex_unlock(&sensor->lock);
}

//-------------------------------------------
// Probe
//-------------------------------------------

static int virtual_sensor_probe(struct platform_device* pdev)
{
    struct virtual_sensor* sensor;
    int temperature;
    int humidity;

    sensor = devm_kzalloc(&pdev->dev,
        sizeof(*sensor),
        GFP_KERNEL);

    if (!sensor)
        return -ENOMEM;

    mutex_init(&sensor->lock);

    sensor->temperature = 2500;
    sensor->humidity = 600;

    platform_set_drvdata(pdev, sensor);

    virtual_sensor_update(sensor);

    virtual_sensor_read(sensor,
        &temperature,
        &humidity);

    dev_info(&pdev->dev,
        "virtual sensor: temperature=%d mC\n",
        temperature);

    dev_info(&pdev->dev,
        "virtual sensor: humidity=%d %%\n",
        humidity);

    return 0;
}

//-------------------------------------------
// Remove
//-------------------------------------------

static void virtual_sensor_remove(struct platform_device* pdev)
{
    dev_info(&pdev->dev,
        "virtual sensor removed\n");
}

//-------------------------------------------
// Device Tree
//-------------------------------------------

static const struct of_device_id virtual_sensor_of_match[] = {
    {.compatible = "bexel,virtual-sensor" },
    { }
};

MODULE_DEVICE_TABLE(of, virtual_sensor_of_match);

//-------------------------------------------
// Platform driver
//-------------------------------------------

static struct platform_driver virtual_sensor_driver = {
    .probe = virtual_sensor_probe,
    .remove = virtual_sensor_remove,

    .driver = {
        .name = "virtual-sensor",
        .of_match_table = virtual_sensor_of_match,
    },
};

module_platform_driver(virtual_sensor_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Virtual sensor platform driver");


/*
//-------------------------------------------



//-------------------------------------------
*/


