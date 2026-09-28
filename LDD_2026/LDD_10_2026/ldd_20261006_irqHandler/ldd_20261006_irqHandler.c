//-------------------------------------------
// ldd_20261006_irqHandler.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int irq = -1;

module_param(irq, int, 0444);

static unsigned long interruptCount;

//-------------------------------------------

static irqreturn_t my_irq_handler(
    int irq,
    void* dev_id)
{
    interruptCount++;

    printk(KERN_INFO
        "irqHandler: interrupt #%lu\n",
        interruptCount);

    /*
     * Return IRQ_HANDLED when this device
     * handled the interrupt.
     */

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init irq_handler_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_irqHandler: init\n");

    if (irq < 0)
        return -EINVAL;

    ret = request_irq(
        irq,
        my_irq_handler,
        0,
        "ldd_irq_handler",
        &irq
    );

    if (ret)
        return ret;

    return 0;
}

//-------------------------------------------

static void __exit irq_handler_exit(void)
{
    free_irq(irq, &irq);

    printk(KERN_INFO
        "Total interrupts: %lu\n",
        interruptCount);

    printk(KERN_INFO
        "ldd_20261006_irqHandler: exit\n");
}

//-------------------------------------------

module_init(irq_handler_init);
module_exit(irq_handler_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("IRQ handler demonstration");

//-------------------------------------------


