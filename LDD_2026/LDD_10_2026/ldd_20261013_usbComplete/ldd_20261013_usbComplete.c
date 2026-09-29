#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * Complete USB driver demonstration.
 */

 //-------------------------------------------

struct usb_complete_data
{
    struct usb_device* udev;

    struct usb_interface* interface;

    struct urb* urb;

    unsigned char* buffer;

    dma_addr_t dma;

    unsigned int buffer_size;
};

//-------------------------------------------

static void usb_complete_callback(struct urb* urb)
{
    struct usb_complete_data* data;

    data = urb->context;

    if (urb->status == 0)
    {
        pr_info("usbComplete: transfer successful\n");

        pr_info("usbComplete: received %d bytes\n",
            urb->actual_length);
    }
    else
    {
        pr_err("usbComplete: URB status = %d\n",
            urb->status);
    }
}

//-------------------------------------------

static int usb_complete_probe(
    struct usb_interface* interface,
    const struct usb_device_id* id)
{
    struct usb_complete_data* data;

    struct usb_host_interface* iface_desc;
    struct usb_endpoint_descriptor* endpoint;

    int i;
    int ret;

    data = devm_kzalloc(&interface->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->udev = interface_to_usbdev(interface);

    data->interface = interface;

    iface_desc = interface->cur_altsetting;

    for (i = 0;
        i < iface_desc->desc.bNumEndpoints;
        i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_int_in(endpoint))
        {
            data->buffer_size =
                usb_endpoint_maxp(endpoint);

            data->buffer =
                usb_alloc_coherent(
                    data->udev,
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
                    data->udev,
                    data->buffer_size,
                    data->buffer,
                    data->dma);

                return -ENOMEM;
            }

            usb_fill_int_urb(
                data->urb,
                data->udev,
                usb_rcvintpipe(
                    data->udev,
                    endpoint->bEndpointAddress),
                data->buffer,
                data->buffer_size,
                usb_complete_callback,
                data,
                endpoint->bInterval);

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
                    data->udev,
                    data->buffer_size,
                    data->buffer,
                    data->dma);

                return ret;
            }

            pr_info("usbComplete: driver initialized\n");

            return 0;
        }
    }

    return -ENODEV;
}

//-------------------------------------------

static void usb_complete_disconnect(
    struct usb_interface* interface)
{
    struct usb_complete_data* data;

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

    pr_info("usbComplete: driver disconnected\n");
}

//-------------------------------------------

static const struct usb_device_id usb_complete_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb, usb_complete_table);

//-------------------------------------------

static struct usb_driver usb_complete_driver =
{
    .name = "ldd_usb_complete",

    .probe = usb_complete_probe,
    .disconnect = usb_complete_disconnect,

    .id_table = usb_complete_table,
};

//-------------------------------------------

module_usb_driver(usb_complete_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete USB driver demonstration");



