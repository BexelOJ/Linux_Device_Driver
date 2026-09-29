#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Network device TX example");

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
    //-------------------------------------------
    // Packet information
    //-------------------------------------------

    pr_info("-------------------------------------------\n");
    pr_info("ldd_netdevTX: TX packet\n");

    pr_info("length = %u bytes\n",
        skb->len);

    pr_info("headroom = %u\n",
        skb_headroom(skb));

    //-------------------------------------------
    // A real driver would:
    //
    // 1. Map skb data for DMA
    // 2. Put DMA address into TX descriptor
    // 3. Notify hardware
    // 4. Hardware transmits packet
    //
    //-------------------------------------------

    dev->stats.tx_packets++;
    dev->stats.tx_bytes += skb->len;

    //-------------------------------------------
    // This example has no hardware.
    //-------------------------------------------

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

    ldd_netdev = alloc_etherdev(0);

    if (!ldd_netdev)
        return -ENOMEM;

    //-------------------------------------------

    strscpy(ldd_netdev->name,
        "lddtx%d",
        IFNAMSIZ);

    ldd_netdev->netdev_ops = &ldd_ops;

    eth_hw_addr_random(ldd_netdev);

    //-------------------------------------------

    ret = register_netdev(ldd_netdev);

    if (ret) {
        free_netdev(ldd_netdev);
        return ret;
    }

    pr_info("ldd_netdevTX loaded\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    unregister_netdev(ldd_netdev);
    free_netdev(ldd_netdev);

    pr_info("ldd_netdevTX removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------



