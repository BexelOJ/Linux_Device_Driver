#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Network device open and close example");

//-------------------------------------------

static struct net_device* ldd_netdev;

//-------------------------------------------

static int ldd_open(struct net_device* dev)
{
    pr_info("ldd_netdevOpenClose: ndo_open()\n");

    //-------------------------------------------
    // Start TX queue
    //-------------------------------------------

    netif_start_queue(dev);

    //-------------------------------------------

    netif_carrier_on(dev);

    pr_info("Network device is UP\n");

    return 0;
}

//-------------------------------------------

static int ldd_close(struct net_device* dev)
{
    pr_info("ldd_netdevOpenClose: ndo_stop()\n");

    //-------------------------------------------

    netif_stop_queue(dev);

    netif_carrier_off(dev);

    pr_info("Network device is DOWN\n");

    return 0;
}

//-------------------------------------------

static netdev_tx_t ldd_xmit(
    struct sk_buff* skb,
    struct net_device* dev)
{
    pr_info("Packet transmitted\n");

    dev_kfree_skb(skb);

    return NETDEV_TX_OK;
}

//-------------------------------------------

static const struct net_device_ops ldd_ops = {
    .ndo_open = ldd_open,
    .ndo_stop = ldd_close,
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
        "lddopen%d",
        IFNAMSIZ);

    ldd_netdev->netdev_ops = &ldd_ops;

    eth_hw_addr_random(ldd_netdev);

    //-------------------------------------------

    ret = register_netdev(ldd_netdev);

    if (ret) {
        free_netdev(ldd_netdev);
        return ret;
    }

    pr_info("ldd_netdevOpenClose loaded\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    unregister_netdev(ldd_netdev);
    free_netdev(ldd_netdev);

    pr_info("ldd_netdevOpenClose removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------



