#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * Demonstrates information available through
 * struct usb_device.
 */

 //-------------------------------------------

static int usb_device_probe(struct usb_interface* interface,
    const struct usb_device_id* id)
{
    struct usb_device* udev;

    udev = interface_to_usbdev(interface);

    pr_info("usbDevice: USB device detected\n");

    pr_info("usbDevice: vendor ID = 0x%04X\n",
        le16_to_cpu(udev->descriptor.idVendor));

    pr_info("usbDevice: product ID = 0x%04X\n",
        le16_to_cpu(udev->descriptor.idProduct));

    pr_info("usbDevice: bus number = %d\n",
        udev->bus->busnum);

    pr_info("usbDevice: device number = %d\n",
        udev->devnum);

    pr_info("usbDevice: speed = %d\n",
        udev->speed);

    return 0;
}

//-------------------------------------------

static void usb_device_disconnect(struct usb_interface* interface)
{
    pr_info("usbDevice: USB device disconnected\n");
}

//-------------------------------------------

static const struct usb_device_id usb_device_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb, usb_device_table);

//-------------------------------------------

static struct usb_driver usb_device_driver =
{
    .name = "ldd_usb_device",

    .probe = usb_device_probe,
    .disconnect = usb_device_disconnect,

    .id_table = usb_device_table,
};

//-------------------------------------------

module_usb_driver(usb_device_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("USB device information demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


