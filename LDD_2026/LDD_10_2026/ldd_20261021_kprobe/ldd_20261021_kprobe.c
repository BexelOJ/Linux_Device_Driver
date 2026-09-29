#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tracepoint.h>

//-------------------------------------------

static int __init tracepoints_init(void)
{
    pr_info("tracepoints: module loaded\n");

    pr_info("tracepoints: available kernel tracepoints can be inspected\n");

    return 0;
}

//-------------------------------------------

static void __exit tracepoints_exit(void)
{
    pr_info("tracepoints: module unloaded\n");
}

//-------------------------------------------

module_init(tracepoints_init);
module_exit(tracepoints_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux tracepoints learning example");


