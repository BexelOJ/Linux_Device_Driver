#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * struct usb_driver represents the
 * software USB driver.
 */

 //-------------------------------------------

static int usb_driver_probe(
    struct usb_interface* interface,
    const struct usb_device_id* id)
{
    pr_info("usbDriver: probe()\n");

    pr_info("usbDriver: vendor = 0x%04X\n",
        id->idVendor);

    pr_info("usbDriver: product = 0x%04X\n",
        id->idProduct);

    return 0;
}

//-------------------------------------------

static void usb_driver_disconnect(
    struct usb_interface* interface)
{
    pr_info("usbDriver: disconnect()\n");
}

//-------------------------------------------

static const struct usb_device_id usb_driver_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb, usb_driver_table);

//-------------------------------------------

static struct usb_driver usb_driver =
{
    .name = "ldd_usb_driver",

    .probe = usb_driver_probe,
    .disconnect = usb_driver_disconnect,

    .id_table = usb_driver_table,
};

//-------------------------------------------

module_usb_driver(usb_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic USB driver structure");


/*
//-------------------------------------------



//-------------------------------------------
*/


