#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * URB = USB Request Block
 *
 * It represents one asynchronous USB transfer request.
 */

 //-------------------------------------------

struct usb_urb_data
{
    struct urb* urb;
};

//-------------------------------------------

static void usb_urb_complete(struct urb* urb)
{
    pr_info("usbURB: completion callback\n");

    pr_info("usbURB: status = %d\n",
        urb->status);

    pr_info("usbURB: actual length = %d\n",
        urb->actual_length);
}

//-------------------------------------------

static int usb_urb_probe(struct usb_interface* interface,
    const struct usb_device_id* id)
{
    struct usb_urb_data* data;

    data = devm_kzalloc(&interface->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->urb = usb_alloc_urb(0, GFP_KERNEL);

    if (!data->urb)
        return -ENOMEM;

    /*
     * This example demonstrates URB allocation.
     *
     * A real URB must be configured with an endpoint,
     * transfer buffer and transfer length before
     * usb_submit_urb().
     */

    usb_set_intfdata(interface, data);

    pr_info("usbURB: URB allocated successfully\n");

    return 0;
}

//-------------------------------------------

static void usb_urb_disconnect(struct usb_interface* interface)
{
    struct usb_urb_data* data;

    data = usb_get_intfdata(interface);

    usb_set_intfdata(interface, NULL);

    if (!data)
        return;

    if (data->urb)
        usb_free_urb(data->urb);

    pr_info("usbURB: URB freed\n");
}

//-------------------------------------------

static const struct usb_device_id usb_urb_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb, usb_urb_table);

//-------------------------------------------

static struct usb_driver usb_urb_driver =
{
    .name = "ldd_usb_urb",

    .probe = usb_urb_probe,
    .disconnect = usb_urb_disconnect,

    .id_table = usb_urb_table,
};

//-------------------------------------------

module_usb_driver(usb_urb_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("USB URB demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


