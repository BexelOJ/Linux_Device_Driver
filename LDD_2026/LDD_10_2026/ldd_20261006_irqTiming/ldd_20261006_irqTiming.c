//-------------------------------------------
// ldd_20261006_irqTiming.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ktime.h>

static int irq = -1;

module_param(irq, int, 0444);

static u64 interruptCount;

//-------------------------------------------

static irqreturn_t irq_timing_handler(
    int irq,
    void* dev_id)
{
    u64 timestamp;

    timestamp = ktime_get_ns();

    interruptCount++;

    printk(KERN_INFO
        "irqTiming: count=%llu time=%llu ns\n",
        interruptCount,
        timestamp);

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init irq_timing_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_irqTiming: init\n");

    if (irq < 0)
        return -EINVAL;

    ret = request_irq(
        irq,
        irq_timing_handler,
        0,
        "ldd_irq_timing",
        &irq
    );

    if (ret)
        return ret;

    return 0;
}

//-------------------------------------------

static void __exit irq_timing_exit(void)
{
    free_irq(irq, &irq);

    printk(KERN_INFO
        "Total interrupts: %llu\n",
        interruptCount);

    printk(KERN_INFO
        "ldd_20261006_irqTiming: exit\n");
}

//-------------------------------------------

module_init(irq_timing_init);
module_exit(irq_timing_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("IRQ timing demonstration");

//-------------------------------------------



