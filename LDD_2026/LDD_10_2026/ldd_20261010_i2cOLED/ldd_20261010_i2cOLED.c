#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/i2c.h>

//-------------------------------------------

#define OLED_CMD    0x00
#define OLED_DATA   0x40

//-------------------------------------------

static int oled_send_command(struct i2c_client* client,
    u8 command)
{
    u8 buffer[2];

    buffer[0] = OLED_CMD;
    buffer[1] = command;

    return i2c_master_send(client,
        buffer,
        sizeof(buffer));
}

//-------------------------------------------

static int oled_probe(struct i2c_client* client)
{
    int ret;

    dev_info(&client->dev,
        "OLED I2C device detected\n");

    //-------------------------------------------
    // Example command
    //-------------------------------------------

    ret = oled_send_command(client,
        0xAE);

    if (ret < 0) {
        dev_err(&client->dev,
            "OLED command failed\n");

        return ret;
    }

    dev_info(&client->dev,
        "OLED command transmitted\n");

    return 0;
}

//-------------------------------------------

static void oled_remove(struct i2c_client* client)
{
    dev_info(&client->dev,
        "OLED driver removed\n");
}

//-------------------------------------------

static const struct of_device_id oled_match[] = {
    {
        .compatible = "ldd,ssd1306-demo",
    },
    { }
};

MODULE_DEVICE_TABLE(of, oled_match);

//-------------------------------------------

static struct i2c_driver oled_driver = {
    .driver = {
        .name = "ldd_i2c_oled",
        .of_match_table = oled_match,
    },

    .probe = oled_probe,
    .remove = oled_remove,
};

//-------------------------------------------

module_i2c_driver(oled_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("I2C OLED example");



