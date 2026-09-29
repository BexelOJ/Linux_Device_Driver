#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * USB endpoint discovery.
 */

 //-------------------------------------------

static int usb_endpoints_probe(
    struct usb_interface* interface,
    const struct usb_device_id* id)
{
    struct usb_host_interface* iface_desc;
    struct usb_endpoint_descriptor* endpoint;

    int i;

    iface_desc = interface->cur_altsetting;

    pr_info("usbEndpoints: interface has %d endpoints\n",
        iface_desc->desc.bNumEndpoints);

    for (i = 0;
        i < iface_desc->desc.bNumEndpoints;
        i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        pr_info("usbEndpoints: endpoint = 0x%02X\n",
            endpoint->bEndpointAddress);

        pr_info("usbEndpoints: max packet = %u\n",
            usb_endpoint_maxp(endpoint));

        if (usb_endpoint_dir_in(endpoint))
            pr_info("usbEndpoints: direction = IN\n");
        else
            pr_info("usbEndpoints: direction = OUT\n");

        if (usb_endpoint_xfer_bulk(endpoint))
            pr_info("usbEndpoints: type = BULK\n");

        if (usb_endpoint_xfer_int(endpoint))
            pr_info("usbEndpoints: type = INTERRUPT\n");

        if (usb_endpoint_xfer_control(endpoint))
            pr_info("usbEndpoints: type = CONTROL\n");

        if (usb_endpoint_xfer_isoc(endpoint))
            pr_info("usbEndpoints: type = ISOCHRONOUS\n");
    }

    return 0;
}

//-------------------------------------------

static void usb_endpoints_disconnect(
    struct usb_interface* interface)
{
    pr_info("usbEndpoints: disconnect()\n");
}

//-------------------------------------------

static const struct usb_device_id usb_endpoints_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb, usb_endpoints_table);

//-------------------------------------------

static struct usb_driver usb_endpoints_driver =
{
    .name = "ldd_usb_endpoints",

    .probe = usb_endpoints_probe,
    .disconnect = usb_endpoints_disconnect,

    .id_table = usb_endpoints_table,
};

//-------------------------------------------

module_usb_driver(usb_endpoints_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("USB endpoint discovery demonstration");



