#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/notifier.h>

#define DRIVER_NAME "ldd_20261023_kernelNotifier"

static int ldd_notifier_callback(struct notifier_block* nb,
    unsigned long event,
    void* data)
{
    pr_info("%s: notifier event=%lu\n",
        DRIVER_NAME, event);

    return NOTIFY_OK;
}

static struct notifier_block ldd_notifier = {
    .notifier_call = ldd_notifier_callback,
};

static int __init kernelNotifier_init(void)
{
    int ret;

    ret = register_reboot_notifier(&ldd_notifier);

    if (ret) {
        pr_err("%s: notifier registration failed\n",
            DRIVER_NAME);
        return ret;
    }

    pr_info("%s: reboot notifier registered\n",
        DRIVER_NAME);

    return 0;
}

static void __exit kernelNotifier_exit(void)
{
    unregister_reboot_notifier(&ldd_notifier);

    pr_info("%s: reboot notifier unregistered\n",
        DRIVER_NAME);
}

module_init(kernelNotifier_init);
module_exit(kernelNotifier_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Kernel notifier example");


/*
//-------------------------------------------



//-------------------------------------------
*/


