#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * USB probe/disconnect lifecycle.
 *
 * Device connected:
 *
 *     USB core
 *         ↓
 *       match
 *         ↓
 *      probe()
 *
 * Device removed:
 *
 *     USB core
 *         ↓
 *     disconnect()
 */

 //-------------------------------------------

static int usb_probe_disconnect_probe(
    struct usb_interface* interface,
    const struct usb_device_id* id)
{
    struct usb_device* udev;

    udev = interface_to_usbdev(interface);

    pr_info("usbProbeDisconnect: probe()\n");

    pr_info("usbProbeDisconnect: "
        "VID = 0x%04X\n",
        le16_to_cpu(udev->descriptor.idVendor));

    pr_info("usbProbeDisconnect: "
        "PID = 0x%04X\n",
        le16_to_cpu(udev->descriptor.idProduct));

    usb_set_intfdata(interface,
        (void*)0x12345678);

    return 0;
}

//-------------------------------------------

static void usb_probe_disconnect_disconnect(
    struct usb_interface* interface)
{
    void* data;

    data = usb_get_intfdata(interface);

    usb_set_intfdata(interface, NULL);

    pr_info("usbProbeDisconnect: disconnect()\n");

    if (data)
        pr_info("usbProbeDisconnect: "
            "private data existed\n");
}

//-------------------------------------------

static const struct usb_device_id usb_probe_disconnect_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb,
    usb_probe_disconnect_table);

//-------------------------------------------

static struct usb_driver usb_probe_disconnect_driver =
{
    .name = "ldd_usb_probe_disconnect",

    .probe = usb_probe_disconnect_probe,

    .disconnect =
        usb_probe_disconnect_disconnect,

    .id_table =
        usb_probe_disconnect_table,
};

//-------------------------------------------

module_usb_driver(usb_probe_disconnect_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("USB probe and disconnect lifecycle");


/*
//-------------------------------------------



//-------------------------------------------
*/


