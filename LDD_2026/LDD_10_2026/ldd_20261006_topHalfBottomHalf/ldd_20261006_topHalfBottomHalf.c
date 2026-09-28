//-------------------------------------------
// ldd_20261006_topHalfBottomHalf.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

static int irq = -1;

module_param(irq, int, 0444);

static struct work_struct bottomHalfWork;

//-------------------------------------------
// Bottom half
//-------------------------------------------

static void bottom_half_function(
    struct work_struct* work)
{
    printk(KERN_INFO
        "topBottom: bottom half executed\n");
}

//-------------------------------------------
// Top half
//-------------------------------------------

static irqreturn_t top_half_handler(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "topBottom: top half executed\n");

    schedule_work(&bottomHalfWork);

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init top_bottom_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_topHalfBottomHalf: init\n");

    if (irq < 0)
        return -EINVAL;

    INIT_WORK(
        &bottomHalfWork,
        bottom_half_function
    );

    ret = request_irq(
        irq,
        top_half_handler,
        0,
        "ldd_top_bottom",
        &irq
    );

    if (ret)
        return ret;

    return 0;
}

//-------------------------------------------

static void __exit top_bottom_exit(void)
{
    free_irq(irq, &irq);

    flush_work(&bottomHalfWork);

    printk(KERN_INFO
        "ldd_20261006_topHalfBottomHalf: exit\n");
}

//-------------------------------------------

module_init(top_bottom_init);
module_exit(top_bottom_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("IRQ top half and bottom half demonstration");

//-------------------------------------------


