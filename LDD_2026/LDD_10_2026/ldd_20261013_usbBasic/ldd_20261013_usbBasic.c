#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * Basic USB driver.
 */

 //-------------------------------------------

static int usb_basic_probe(struct usb_interface* interface,
    const struct usb_device_id* id)
{
    pr_info("usbBasic: USB device matched\n");

    pr_info("usbBasic: vendor = 0x%04X\n",
        id->idVendor);

    pr_info("usbBasic: product = 0x%04X\n",
        id->idProduct);

    pr_info("usbBasic: interface number = %d\n",
        interface->cur_altsetting->desc.bInterfaceNumber);

    return 0;
}

//-------------------------------------------

static void usb_basic_disconnect(struct usb_interface* interface)
{
    pr_info("usbBasic: USB device disconnected\n");
}

//-------------------------------------------

static const struct usb_device_id usb_basic_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb, usb_basic_table);

//-------------------------------------------

static struct usb_driver usb_basic_driver =
{
    .name = "ldd_usb_basic",

    .probe = usb_basic_probe,
    .disconnect = usb_basic_disconnect,

    .id_table = usb_basic_table,
};

//-------------------------------------------

module_usb_driver(usb_basic_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic USB driver");



