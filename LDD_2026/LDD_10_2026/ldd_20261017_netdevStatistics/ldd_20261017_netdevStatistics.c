#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/u64_stats_sync.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Network device statistics example");

//-------------------------------------------

struct ldd_stats {
    u64 rx_packets;
    u64 rx_bytes;

    u64 tx_packets;
    u64 tx_bytes;

    u64 rx_dropped;
    u64 tx_dropped;

    struct u64_stats_sync sync;
};

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
    struct ldd_stats* stats =
        netdev_priv(dev);

    //-------------------------------------------

    u64_stats_update_begin(&stats->sync);

    stats->tx_packets++;
    stats->tx_bytes += skb->len;

    u64_stats_update_end(&stats->sync);

    //-------------------------------------------

    dev_kfree_skb(skb);

    return NETDEV_TX_OK;
}

//-------------------------------------------

static void ldd_get_stats64(
    struct net_device* dev,
    struct rtnl_link_stats64* stats)
{
    struct ldd_stats* priv =
        netdev_priv(dev);

    unsigned int start;

    //-------------------------------------------

    do {

        start =
            u64_stats_fetch_begin(&priv->sync);

        stats->rx_packets =
            priv->rx_packets;

        stats->rx_bytes =
            priv->rx_bytes;

        stats->tx_packets =
            priv->tx_packets;

        stats->tx_bytes =
            priv->tx_bytes;

        stats->rx_dropped =
            priv->rx_dropped;

        stats->tx_dropped =
            priv->tx_dropped;

    } while (u64_stats_fetch_retry(&priv->sync,
        start));
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
    struct ldd_stats* stats;
    int ret;

    //-------------------------------------------

    ldd_netdev =
        alloc_etherdev(sizeof(*stats));

    if (!ldd_netdev)
        return -ENOMEM;

    //-------------------------------------------

    stats = netdev_priv(ldd_netdev);

    memset(stats, 0, sizeof(*stats));

    u64_stats_init(&stats->sync);

    //-------------------------------------------

    strscpy(ldd_netdev->name,
        "lddstats%d",
        IFNAMSIZ);

    ldd_netdev->netdev_ops = &ldd_ops;

    eth_hw_addr_random(ldd_netdev);

    //-------------------------------------------

    ret = register_netdev(ldd_netdev);

    if (ret) {
        free_netdev(ldd_netdev);
        return ret;
    }

    pr_info("ldd_netdevStatistics loaded\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    unregister_netdev(ldd_netdev);
    free_netdev(ldd_netdev);

    pr_info("ldd_netdevStatistics removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


