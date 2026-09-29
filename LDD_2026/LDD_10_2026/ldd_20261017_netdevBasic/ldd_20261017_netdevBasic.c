#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic Linux network device");

MODULE_ALIAS("netdev:lddbasic%d");

//-------------------------------------------

static struct net_device* ldd_netdev;

//-------------------------------------------

static int ldd_open(struct net_device* dev)
{
    pr_info("ldd_netdevBasic: open\n");

    netif_start_queue(dev);
    netif_carrier_on(dev);

    return 0;
}

//-------------------------------------------

static int ldd_stop(struct net_device* dev)
{
    pr_info("ldd_netdevBasic: close\n");

    netif_stop_queue(dev);
    netif_carrier_off(dev);

    return 0;
}

//-------------------------------------------

static netdev_tx_t ldd_xmit(
    struct sk_buff* skb,
    struct net_device* dev)
{
    pr_info("ldd_netdevBasic: packet TX\n");

    pr_info("Packet length = %u\n",
        skb->len);

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

static void ldd_setup(struct net_device* dev)
{
    //-------------------------------------------
    // Ethernet device setup
    //-------------------------------------------

    ether_setup(dev);

    //-------------------------------------------

    dev->netdev_ops = &ldd_ops;

    //-------------------------------------------

    eth_hw_addr_random(dev);
}

//-------------------------------------------

static int __init ldd_init(void)
{
    int ret;

    //-------------------------------------------

    ldd_netdev =
        alloc_netdev(0,
            "lddbasic%d",
            NET_NAME_UNKNOWN,
            ldd_setup);

    if (!ldd_netdev)
        return -ENOMEM;

    //-------------------------------------------

    ret = register_netdev(ldd_netdev);

    if (ret) {

        free_netdev(ldd_netdev);

        return ret;
    }

    //-------------------------------------------

    pr_info("ldd_netdevBasic loaded\n");
    pr_info("Interface: %s\n",
        ldd_netdev->name);

    return 0;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    unregister_netdev(ldd_netdev);

    free_netdev(ldd_netdev);

    pr_info("ldd_netdevBasic removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


