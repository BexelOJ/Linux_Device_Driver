//-------------------------------------------
// ldd_20261006_waitQueueIRQ.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/wait.h>

static int irq = -1;

module_param(irq, int, 0444);

static DECLARE_WAIT_QUEUE_HEAD(irqWaitQueue);

static bool irqReceived;

//-------------------------------------------

static irqreturn_t irq_handler(
    int irq,
    void* dev_id)
{
    irqReceived = true;

    wake_up_interruptible(&irqWaitQueue);

    printk(KERN_INFO
        "waitQueueIRQ: interrupt received\n");

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init wait_queue_irq_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_waitQueueIRQ: init\n");

    if (irq < 0)
        return -EINVAL;

    irqReceived = false;

    ret = request_irq(
        irq,
        irq_handler,
        0,
        "ldd_waitqueue_irq",
        &irq
    );

    if (ret)
        return ret;

    printk(KERN_INFO
        "IRQ registered\n");

    /*
     * The actual driver would normally have a
     * read() operation that waits here.
     */

    printk(KERN_INFO
        "Application would wait using read()\n");

    return 0;
}

//-------------------------------------------

static void __exit wait_queue_irq_exit(void)
{
    free_irq(irq, &irq);

    printk(KERN_INFO
        "ldd_20261006_waitQueueIRQ: exit\n");
}

//-------------------------------------------

module_init(wait_queue_irq_init);
module_exit(wait_queue_irq_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("IRQ and wait queue demonstration");

//-------------------------------------------


