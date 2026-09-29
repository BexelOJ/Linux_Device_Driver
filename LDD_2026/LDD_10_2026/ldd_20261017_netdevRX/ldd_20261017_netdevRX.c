#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Network device RX example");

//-------------------------------------------

static struct net_device* ldd_netdev;

//-------------------------------------------

static int ldd_receive_packet(
    struct net_device* dev,
    const void* data,
    unsigned int len)
{
    struct sk_buff* skb;

    //-------------------------------------------
    // Allocate skb
    //-------------------------------------------

    skb = netdev_alloc_skb(dev, len);

    if (!skb) {
        dev->stats.rx_dropped++;
        return -ENOMEM;
    }

    //-------------------------------------------
    // Copy received data
    //-------------------------------------------

    memcpy(skb_put(skb, len),
        data,
        len);

    //-------------------------------------------
    // Tell networking stack protocol type
    //-------------------------------------------

    skb->protocol =
        eth_type_trans(skb, dev);

    //-------------------------------------------

    dev->stats.rx_packets++;
    dev->stats.rx_bytes += len;

    //-------------------------------------------
    // Pass packet to networking stack
    //-------------------------------------------

    netif_rx(skb);

    return 0;
}

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
        "lddrx%d",
        IFNAMSIZ);

    ldd_netdev->netdev_ops = &ldd_ops;

    eth_hw_addr_random(ldd_netdev);

    //-------------------------------------------

    ret = register_netdev(ldd_netdev);

    if (ret) {
        free_netdev(ldd_netdev);
        return ret;
    }

    pr_info("ldd_netdevRX loaded\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    unregister_netdev(ldd_netdev);
    free_netdev(ldd_netdev);

    pr_info("ldd_netdevRX removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


