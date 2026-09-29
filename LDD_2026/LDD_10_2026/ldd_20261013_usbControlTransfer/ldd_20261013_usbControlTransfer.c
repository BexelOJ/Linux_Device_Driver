#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * USB control transfer demonstration.
 */

 //-------------------------------------------

static int usb_control_probe(
    struct usb_interface* interface,
    const struct usb_device_id* id)
{
    struct usb_device* udev;

    u8 request_type;
    u8 request;

    u16 value;
    u16 index;

    int ret;

    u8 buffer[4];

    udev = interface_to_usbdev(interface);

    /*
     * Example:
     *
     * Standard USB GET_STATUS request.
     */

    request_type =
        USB_DIR_IN |
        USB_TYPE_STANDARD |
        USB_RECIP_DEVICE;

    request = USB_REQ_GET_STATUS;

    value = 0;
    index = 0;

    ret = usb_control_msg(
        udev,
        usb_rcvctrlpipe(udev, 0),
        request,
        request_type,
        value,
        index,
        buffer,
        sizeof(buffer),
        USB_CTRL_GET_TIMEOUT);

    if (ret < 0)
    {
        pr_err("usbControlTransfer: "
            "usb_control_msg() failed: %d\n",
            ret);

        return ret;
    }

    pr_info("usbControlTransfer: "
        "control transfer completed\n");

    pr_info("usbControlTransfer: "
        "returned %d bytes\n",
        ret);

    return 0;
}

//-------------------------------------------

static void usb_control_disconnect(
    struct usb_interface* interface)
{
    pr_info("usbControlTransfer: disconnect()\n");
}

//-------------------------------------------

static const struct usb_device_id usb_control_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb, usb_control_table);

//-------------------------------------------

static struct usb_driver usb_control_driver =
{
    .name = "ldd_usb_control",

    .probe = usb_control_probe,
    .disconnect = usb_control_disconnect,

    .id_table = usb_control_table,
};

//-------------------------------------------

module_usb_driver(usb_control_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("USB control transfer demonstration");



