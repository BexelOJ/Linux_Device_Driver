#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>

#define DRIVER_NAME "ldd_20261023_uevent"

static struct kobject* ldd_kobj;

static int __init uevent_init(void)
{
    int ret;

    ldd_kobj = kobject_create_and_add(DRIVER_NAME, kernel_kobj);

    if (!ldd_kobj)
        return -ENOMEM;

    ret = kobject_uevent(ldd_kobj, KOBJ_ADD);

    if (ret) {
        pr_err("%s: kobject_uevent failed\n", DRIVER_NAME);
        kobject_put(ldd_kobj);
        return ret;
    }

    pr_info("%s: KOBJ_ADD uevent generated\n", DRIVER_NAME);

    return 0;
}

static void __exit uevent_exit(void)
{
    kobject_uevent(ldd_kobj, KOBJ_REMOVE);
    kobject_put(ldd_kobj);

    pr_info("%s: KOBJ_REMOVE uevent generated\n", DRIVER_NAME);
}

module_init(uevent_init);
module_exit(uevent_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Kernel uevent example");


/*
//-------------------------------------------



//-------------------------------------------
*/


