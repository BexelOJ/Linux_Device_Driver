#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/dma-mapping.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Network device DMA mapping example");

//-------------------------------------------

static struct net_device* ldd_netdev;

//-------------------------------------------

static int ldd_open(struct net_device* dev)
{
    netif_start_queue(dev);
    netif_carrier_on(dev);

    return 0;
}

//-------------------------------------------

static int ldd_stop(struct net_device* dev)
{
    netif_stop_queue(dev);
    netif_carrier_off(dev);

    return 0;
}

//-------------------------------------------

static netdev_tx_t ldd_xmit(
    struct sk_buff* skb,
    struct net_device* dev)
{
    dma_addr_t dma_address;
    struct device* device = &dev->dev;

    //-------------------------------------------

    pr_info("ldd_netdevDMA: TX packet\n");

    //-------------------------------------------
    // Map skb data for DMA
    //-------------------------------------------

    dma_address =
        dma_map_single(device,
            skb->data,
            skb_headlen(skb),
            DMA_TO_DEVICE);

    //-------------------------------------------

    if (dma_mapping_error(device, dma_address)) {

        dev->stats.tx_dropped++;

        dev_kfree_skb(skb);

        return NETDEV_TX_OK;
    }

    //-------------------------------------------

    pr_info("DMA address = %pad\n",
        &dma_address);

    //-------------------------------------------
    // Real hardware:
    //
    // TX descriptor:
    //
    // descriptor->dma_address = dma_address;
    //
    // then hardware is notified.
    //-------------------------------------------

    //-------------------------------------------
    // Since there is no hardware here,
    // immediately unmap.
    //-------------------------------------------

    dma_unmap_single(device,
        dma_address,
        skb_headlen(skb),
        DMA_TO_DEVICE);

    //-------------------------------------------

    dev->stats.tx_packets++;
    dev->stats.tx_bytes += skb->len;

    dev_kfree_skb(skb);

    return NETDEV_TX_OK;
}

//-------------------------------------------

static const struct net_device_ops ldd_ops = {
    .ndo_open = ldd_open,
    .ndo_stop = ldd_stop,
    .ndo_start_xmit = ldd_xmit,
};

//-------------------------------------------

static int __init ldd_init(void)
{
    int ret;

    //-------------------------------------------

    ldd_netdev = alloc_etherdev(0);

    if (!ldd_netdev)
        return -ENOMEM;

    //-------------------------------------------

    strscpy(ldd_netdev->name,
        "ldddma%d",
        IFNAMSIZ);

    ldd_netdev->netdev_ops = &ldd_ops;

    eth_hw_addr_random(ldd_netdev);

    //-------------------------------------------

    ret = register_netdev(ldd_netdev);

    if (ret) {
        free_netdev(ldd_netdev);
        return ret;
    }

    pr_info("ldd_netdevDMA loaded\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    unregister_netdev(ldd_netdev);
    free_netdev(ldd_netdev);

    pr_info("ldd_netdevDMA removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------



/*
//-------------------------------------------

skb
 │
 ├── skb->data
 │
 ▼
dma_map_single()
 │
 ▼
DMA address
 │
 ▼
TX descriptor
 │
 ▼
NIC hardware
 │
 ▼
DMA reads packet
 │
 ▼
Network transmission

//-------------------------------------------
*/


