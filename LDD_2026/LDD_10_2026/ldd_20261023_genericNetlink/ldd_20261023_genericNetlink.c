#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <net/genetlink.h>

#define DRIVER_NAME "ldd_20261023_genericNetlink"

enum {
    LDD_CMD_UNSPEC,
    LDD_CMD_STATUS,
    __LDD_CMD_MAX,
};

#define LDD_CMD_MAX (__LDD_CMD_MAX - 1)

enum {
    LDD_ATTR_UNSPEC,
    LDD_ATTR_MESSAGE,
    __LDD_ATTR_MAX,
};

#define LDD_ATTR_MAX (__LDD_ATTR_MAX - 1)

static struct genl_family ldd_family;

static const struct nla_policy ldd_policy[LDD_ATTR_MAX + 1] = {
    [LDD_ATTR_MESSAGE] = {
        .type = NLA_NUL_STRING,
        .len = 128,
    },
};

static int ldd_status(struct sk_buff* skb,
    struct genl_info* info)
{
    pr_info("%s: STATUS command received\n", DRIVER_NAME);

    return 0;
}

static const struct genl_ops ldd_ops[] = {
    {
        .cmd = LDD_CMD_STATUS,
        .flags = 0,
        .policy = ldd_policy,
        .doit = ldd_status,
    },
};

static struct genl_family ldd_family = {
    .name = DRIVER_NAME,
    .version = 1,
    .maxattr = LDD_ATTR_MAX,
    .module = THIS_MODULE,
    .ops = ldd_ops,
    .n_ops = ARRAY_SIZE(ldd_ops),
};

static int __init genericNetlink_init(void)
{
    int ret;

    ret = genl_register_family(&ldd_family);

    if (ret) {
        pr_err("%s: genl_register_family failed\n", DRIVER_NAME);
        return ret;
    }

    pr_info("%s: Generic Netlink family registered\n", DRIVER_NAME);

    return 0;
}

static void __exit genericNetlink_exit(void)
{
    genl_unregister_family(&ldd_family);

    pr_info("%s: Generic Netlink family unregistered\n", DRIVER_NAME);
}

module_init(genericNetlink_init);
module_exit(genericNetlink_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Generic Netlink example");


/*
//-------------------------------------------



//-------------------------------------------
*/


