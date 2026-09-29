#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netlink.h>
#include <net/sock.h>

#define DRIVER_NAME "ldd_20261023_netlink"
#define NETLINK_LDD 31

static struct sock* ldd_nl_sock;

static void ldd_netlink_receive(struct sk_buff* skb)
{
    struct nlmsghdr* nlh;
    char* message;

    nlh = nlmsg_hdr(skb);

    message = nlmsg_data(nlh);

    pr_info("%s: received: %s\n",
        DRIVER_NAME, message);
}

static int __init netlink_init(void)
{
    struct netlink_kernel_cfg cfg = {
        .input = ldd_netlink_receive,
    };

    ldd_nl_sock = netlink_kernel_create(&init_net,
        NETLINK_LDD,
        &cfg);

    if (!ldd_nl_sock) {
        pr_err("%s: failed to create Netlink socket\n",
            DRIVER_NAME);
        return -ENOMEM;
    }

    pr_info("%s: Netlink socket created\n", DRIVER_NAME);

    return 0;
}

static void __exit netlink_exit(void)
{
    netlink_kernel_release(ldd_nl_sock);

    pr_info("%s: Netlink socket released\n", DRIVER_NAME);
}

module_init(netlink_init);
module_exit(netlink_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic Netlink example");


/*
//-------------------------------------------



//-------------------------------------------
*/


