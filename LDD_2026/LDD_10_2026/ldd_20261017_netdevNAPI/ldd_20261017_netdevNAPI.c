#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Network device NAPI example");

//-------------------------------------------

struct ldd_napi_private {
    struct napi_struct napi;
    struct net_device* netdev;
};

//-------------------------------------------

static int ldd_napi_poll(
    struct napi_struct* napi,
    int budget)
{
    struct ldd_napi_private* priv =
        container_of(napi,
            struct ldd_napi_private,
            napi);

    int work_done = 0;

    //-------------------------------------------

    pr_info("ldd_netdevNAPI: poll()\n");

    //-------------------------------------------
    // A real driver would process RX descriptors:
    //
    // while (work_done < budget) {
    //     process RX descriptor;
    //     create skb;
    //     pass skb upward;
    //     work_done++;
    // }
    //-------------------------------------------

    //-------------------------------------------
    // No hardware packets in this example.
    //-------------------------------------------

    napi_complete_done(napi, work_done);

    return work_done;
}

//-------------------------------------------

static int ldd_open(struct net_device* dev)
{
    struct ldd_napi_private* priv =
        netdev_priv(dev);

    //-------------------------------------------

    napi_enable(&priv->napi);

    netif_start_queue(dev);
    netif_carrier_on(dev);

    return 0;
}

//-------------------------------------------

static int ldd_stop(struct net_device* dev)
{
    struct ldd_napi_private* priv =
        netdev_priv(dev);

    //-------------------------------------------

    netif_stop_queue(dev);
    netif_carrier_off(dev);

    napi_disable(&priv->napi);

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
    struct ldd_napi_private* priv;
    int ret;

    //-------------------------------------------

    ldd_netdev =
        alloc_etherdev(sizeof(*priv));

    if (!ldd_netdev)
        return -ENOMEM;

    //-------------------------------------------

    priv = netdev_priv(ldd_netdev);

    priv->netdev = ldd_netdev;

    //-------------------------------------------
    // Add NAPI
    //-------------------------------------------

    netif_napi_add(ldd_netdev,
        &priv->napi,
        ldd_napi_poll);

    //-------------------------------------------

    strscpy(ldd_netdev->name,
        "lddnapi%d",
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

    pr_info("ldd_netdevNAPI loaded\n");

    return 0;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    struct ldd_napi_private* priv =
        netdev_priv(ldd_netdev);

    unregister_netdev(ldd_netdev);

    netif_napi_del(&priv->napi);

    free_netdev(ldd_netdev);

    pr_info("ldd_netdevNAPI removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------


/*
//-------------------------------------------

             Packet arrives
                  │
                  ▼
                 IRQ
                  │
                  │ schedule
                  ▼
             NAPI poll()
                  │
          ┌───────┴───────┐
          │               │
       packet          packet
          │               │
          └───────┬───────┘
                  ▼
             network stack

//-------------------------------------------
*/


