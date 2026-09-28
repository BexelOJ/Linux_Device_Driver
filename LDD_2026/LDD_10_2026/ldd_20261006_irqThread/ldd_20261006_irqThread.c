//-------------------------------------------
// ldd_20261006_irqThread.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int irq = -1;

module_param(irq, int, 0444);

//-------------------------------------------
// Top half
//-------------------------------------------

static irqreturn_t irq_top_half(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "irqThread: top half\n");

    return IRQ_WAKE_THREAD;
}

//-------------------------------------------
// Threaded handler
//-------------------------------------------

static irqreturn_t irq_thread(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "irqThread: threaded handler\n");

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init irq_thread_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_irqThread: init\n");

    if (irq < 0)
        return -EINVAL;

    ret = request_threaded_irq(
        irq,
        irq_top_half,
        irq_thread,
        0,
        "ldd_irq_thread",
        &irq
    );

    if (ret)
        return ret;

    return 0;
}

//-------------------------------------------

static void __exit irq_thread_exit(void)
{
    free_irq(irq, &irq);

    printk(KERN_INFO
        "ldd_20261006_irqThread: exit\n");
}

//-------------------------------------------

module_init(irq_thread_init);
module_exit(irq_thread_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Threaded IRQ demonstration");

//-------------------------------------------


