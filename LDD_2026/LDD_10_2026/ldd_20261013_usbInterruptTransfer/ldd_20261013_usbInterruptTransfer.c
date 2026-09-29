#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

/*
 * USB interrupt transfer demonstration.
 */

 //-------------------------------------------

struct usb_interrupt_data
{
    struct urb* urb;

    u8* buffer;
    dma_addr_t dma;

    unsigned int pipe;
};

//-------------------------------------------

static void usb_interrupt_complete(struct urb* urb)
{
    struct usb_interrupt_data* data;

    data = urb->context;

    if (urb->status == 0)
    {
        pr_info("usbInterruptTransfer: interrupt data received\n");
    }
    else
    {
        pr_err("usbInterruptTransfer: URB status = %d\n",
            urb->status);
    }
}

//-------------------------------------------

static int usb_interrupt_probe(struct usb_interface* interface,
    const struct usb_device_id* id)
{
    struct usb_device* udev;
    struct usb_host_interface* iface_desc;
    struct usb_endpoint_descriptor* endpoint;

    struct usb_interrupt_data* data;

    int i;
    int ret;

    udev = interface_to_usbdev(interface);

    iface_desc = interface->cur_altsetting;

    data = devm_kzalloc(&interface->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    for (i = 0;
        i < iface_desc->desc.bNumEndpoints;
        i++)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_int_in(endpoint))
        {
            data->pipe =
                usb_rcvintpipe(udev,
                    endpoint->bEndpointAddress);

            data->urb =
                usb_alloc_urb(0, GFP_KERNEL);

            if (!data->urb)
                return -ENOMEM;

            data->buffer =
                usb_alloc_coherent(
                    udev,
                    endpoint->wMaxPacketSize,
                    GFP_KERNEL,
                    &data->dma);

            if (!data->buffer)
            {
                usb_free_urb(data->urb);
                return -ENOMEM;
            }

            usb_fill_int_urb(
                data->urb,
                udev,
                data->pipe,
                data->buffer,
                endpoint->wMaxPacketSize,
                usb_interrupt_complete,
                data,
                endpoint->bInterval);

            data->urb->transfer_dma = data->dma;
            data->urb->transfer_flags |=
                URB_NO_TRANSFER_DMA_MAP;

            ret = usb_submit_urb(data->urb,
                GFP_KERNEL);

            if (ret)
            {
                pr_err("usbInterruptTransfer: "
                    "usb_submit_urb() failed: %d\n",
                    ret);

                usb_free_coherent(
                    udev,
                    endpoint->wMaxPacketSize,
                    data->buffer,
                    data->dma);

                usb_free_urb(data->urb);

                return ret;
            }

            usb_set_intfdata(interface, data);

            pr_info("usbInterruptTransfer: "
                "interrupt URB submitted\n");

            return 0;
        }
    }

    return -ENODEV;
}

//-------------------------------------------

static void usb_interrupt_disconnect(
    struct usb_interface* interface)
{
    struct usb_interrupt_data* data;
    struct usb_device* udev;

    data = usb_get_intfdata(interface);

    usb_set_intfdata(interface, NULL);

    if (!data)
        return;

    udev = interface_to_usbdev(interface);

    usb_kill_urb(data->urb);

    if (data->buffer)
    {
        usb_free_coherent(
            udev,
            data->urb->transfer_buffer_length,
            data->buffer,
            data->dma);
    }

    usb_free_urb(data->urb);

    pr_info("usbInterruptTransfer: disconnected\n");
}

//-------------------------------------------

static const struct usb_device_id usb_interrupt_table[] =
{
    {
        USB_DEVICE(0x1234, 0x5678)
    },

    { }
};

MODULE_DEVICE_TABLE(usb, usb_interrupt_table);

//-------------------------------------------

static struct usb_driver usb_interrupt_driver =
{
    .name = "ldd_usb_interrupt",

    .probe = usb_interrupt_probe,
    .disconnect = usb_interrupt_disconnect,

    .id_table = usb_interrupt_table,
};

//-------------------------------------------

module_usb_driver(usb_interrupt_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("USB interrupt transfer demonstration");


/*
//-------------------------------------------



//-------------------------------------------
*/


