#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Complete virtual network device example");

//-------------------------------------------

struct ldd_netdev_private {
    struct napi_struct napi;

    u64 tx_packets;
    u64 tx_bytes;

    u64 rx_packets;
    u64 rx_bytes;
};

//-------------------------------------------

static struct net_device* ldd_netdev;

//-------------------------------------------

static int ldd_napi_poll(
    struct napi_struct* napi,
    int budget)
{
    //-------------------------------------------
    // Real driver:
    //
    // Process RX descriptors here.
    //-------------------------------------------

    napi_complete_done(napi, 0);

    return 0;
}

//-------------------------------------------

static int ldd_open(struct net_device* dev)
{
    struct ldd_netdev_private* priv =
        netdev_priv(dev);

    //-------------------------------------------

    napi_enable(&priv->napi);

    netif_start_queue(dev);

    netif_carrier_on(dev);

    pr_info("ldd_netdevComplete: opened\n");

    return 0;
}

//-------------------------------------------

static int ldd_stop(struct net_device* dev)
{
    struct ldd_netdev_private* priv =
        netdev_priv(dev);

    //-------------------------------------------

    netif_stop_queue(dev);

    netif_carrier_off(dev);

    napi_disable(&priv->napi);

    pr_info("ldd_netdevComplete: stopped\n");

    return 0;
}

//-------------------------------------------

static netdev_tx_t ldd_xmit(
    struct sk_buff* skb,
    struct net_device* dev)
{
    struct ldd_netdev_private* priv =
        netdev_priv(dev);

    //-------------------------------------------

    priv->tx_packets++;
    priv->tx_bytes += skb->len;

    //-------------------------------------------
    // No real hardware.
    //-------------------------------------------

    dev_kfree_skb(skb);

    return NETDEV_TX_OK;
}

//-------------------------------------------

static void ldd_get_stats64(
    struct net_device* dev,
    struct rtnl_link_stats64* stats)
{
    struct ldd_netdev_private* priv =
        netdev_priv(dev);

    stats->tx_packets = priv->tx_packets;
    stats->tx_bytes = priv->tx_bytes;

    stats->rx_packets = priv->rx_packets;
    stats->rx_bytes = priv->rx_bytes;
}

//-------------------------------------------

static const struct net_device_ops ldd_ops = {

    .ndo_open = ldd_open,

    .ndo_stop = ldd_stop,

    .ndo_start_xmit = ldd_xmit,

    .ndo_get_stats64 = ldd_get_stats64,
};

//-------------------------------------------

static int __init ldd_init(void)
{
    struct ldd_netdev_private* priv;
    int ret;

    //-------------------------------------------

    ldd_netdev =
        alloc_etherdev(sizeof(*priv));

    if (!ldd_netdev)
        return -ENOMEM;

    //-------------------------------------------

    priv = netdev_priv(ldd_netdev);

    //-------------------------------------------

    netif_napi_add(ldd_netdev,
        &priv->napi,
        ldd_napi_poll);

    //-------------------------------------------

    strscpy(ldd_netdev->name,
        "lddcomplete%d",
        IFNAMSIZ);

    ldd_netdev->netdev_ops = &ldd_ops;

    eth_hw_addr_random(ldd_netdev);

    //-------------------------------------------

    ret = register_netdev(ldd_netdev);

    if (ret) {

        netif_napi_del(&priv->napi);

        free_netdev(ldd_netdev);

        return ret;
    }

    //-------------------------------------------

    pr_info("ldd_netdevComplete loaded\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    struct ldd_netdev_private* priv =
        netdev_priv(ldd_netdev);

    unregister_netdev(ldd_netdev);

    netif_napi_del(&priv->napi);

    free_netdev(ldd_netdev);

    pr_info("ldd_netdevComplete removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------



