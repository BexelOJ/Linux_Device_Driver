#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic network device registration example");

//-------------------------------------------

static struct net_device* ldd_netdev;

//-------------------------------------------

static int ldd_netdev_open(struct net_device* dev)
{
    netif_start_queue(dev);

    pr_info("ldd_netdevRegistration: device opened\n");

    return 0;
}

//-------------------------------------------

static int ldd_netdev_stop(struct net_device* dev)
{
    netif_stop_queue(dev);

    pr_info("ldd_netdevRegistration: device stopped\n");

    return 0;
}

//-------------------------------------------

static netdev_tx_t ldd_netdev_xmit(
    struct sk_buff* skb,
    struct net_device* dev)
{
    pr_info("ldd_netdevRegistration: TX packet\n");

    dev_kfree_skb(skb);

    return NETDEV_TX_OK;
}

//-------------------------------------------

static const struct net_device_ops ldd_netdev_ops = {
    .ndo_open = ldd_netdev_open,
    .ndo_stop = ldd_netdev_stop,
    .ndo_start_xmit = ldd_netdev_xmit,
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
        "lddreg%d",
        IFNAMSIZ);

    ldd_netdev->netdev_ops = &ldd_netdev_ops;

    //-------------------------------------------
    // Generate a locally administered MAC
    //-------------------------------------------

    eth_hw_addr_random(ldd_netdev);

    //-------------------------------------------

    ret = register_netdev(ldd_netdev);

    if (ret) {
        pr_err("register_netdev() failed\n");
        free_netdev(ldd_netdev);
        return ret;
    }

    //-------------------------------------------

    pr_info("ldd_netdevRegistration registered\n");
    pr_info("interface = %s\n", ldd_netdev->name);

    return 0;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    unregister_netdev(ldd_netdev);

    free_netdev(ldd_netdev);

    pr_info("ldd_netdevRegistration removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------



