#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * USB bulk transfer demonstration.
 */

 //-------------------------------------------

struct usb_bulk_data
{
    struct usb_device* udev;

    struct urb* urb;

    unsigned char* buffer;

    dma_addr_t dma;

    unsigned int buffer_size;
};

//-------------------------------------------

static void usb_bulk_complete(struct urb* urb)
{
    struct usb_bulk_data* data;

    data = urb->context;

    if (urb->status == 0)
    {
        pr_info("usbBulkTransfer: "
            "bulk transfer completed\n");

        pr_info("usbBulkTransfer: "
            "actual length = %d\n",
            urb->actual_length);
    }
    else
    {
        pr_err("usbBulkTransfer: "
            "URB status = %d\n",
            urb->status);
    }
}

//-------------------------------------------

static int usb_bulk_probe(
    struct usb_interface* interface,
    const struct usb_device_id* id)
{
    struct usb_device* udev;

    struct usb_host_interface* iface_desc;
    struct usb_endpoint_descriptor* endpoint;

    struct usb_bulk_data* data;

    int i;
    int ret;

    udev = interface_to_usbdev(interface);

    iface_desc = interface->cur_altsetting;

    data = devm_kzalloc(&interface->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->udev = udev;

    for (i = 0;
        i < iface_desc->desc.bNumEndpoints;
        i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in(endpoint))
        {
            data->buffer_size =
                usb_endpoint_maxp(endpoint);

            data->buffer =
                usb_alloc_coherent(
                    udev,
                    data->buffer_size,
                    GFP_KERNEL,
                    &data->dma);

            if (!data->buffer)
                return -ENOMEM;

            data->urb =
                usb_alloc_urb(0, GFP_KERNEL);

            if (!data->urb)
            {
                usb_free_coherent(
                    udev,
                    data->buffer_size,
                    data->buffer,
                    data->dma);

                return -ENOMEM;
            }

            usb_fill_bulk_urb(
                data->urb,
                udev,
                usb_rcvbulkpipe(
                    udev,
                    endpoint->bEndpointAddress),
                data->buffer,
                data->buffer_size,
                usb_bulk_complete,
                data);

            data->urb->transfer_dma = data->dma;

            data->urb->transfer_flags |=
                URB_NO_TRANSFER_DMA_MAP;

            usb_set_intfdata(interface, data);

            ret = usb_submit_urb(data->urb,
                GFP_KERNEL);

            if (ret)
            {
                usb_set_intfdata(interface, NULL);

                usb_free_urb(data->urb);

                usb_free_coherent(
                    udev,
                    data->buffer_size,
                    data->buffer,
                    data->dma);

                return ret;
            }

            pr_info("usbBulkTransfer: "
                "bulk IN URB submitted\n");

            return 0;
        }
    }

    return -ENODEV;
}

//-------------------------------------------

static void usb_bulk_disconnect(
    struct usb_interface* interface)
{
    struct usb_bulk_data* data;

    data = usb_get_intfdata(interface);

    usb_set_intfdata(interface, NULL);

    if (!data)
        return;

    usb_kill_urb(data->urb);

    usb_free_urb(data->urb);

    usb_free_coherent(
        data->udev,
        data->buffer_size,
        data->buffer,
        data->dma);

    pr_info("usbBulkTransfer: disconnected\n");
}

//-------------------------------------------

static const struct usb_device_id usb_bulk_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb, usb_bulk_table);

//-------------------------------------------

static struct usb_driver usb_bulk_driver =
{
    .name = "ldd_usb_bulk",

    .probe = usb_bulk_probe,
    .disconnect = usb_bulk_disconnect,

    .id_table = usb_bulk_table,
};

//-------------------------------------------

module_usb_driver(usb_bulk_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("USB bulk transfer demonstration");



