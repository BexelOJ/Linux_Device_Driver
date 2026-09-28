//-------------------------------------------
// ldd_20261006_irqBasic.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int irq = -1;

module_param(irq, int, 0444);

//-------------------------------------------

static irqreturn_t irq_handler(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "irqBasic: interrupt received\n");

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init irq_basic_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_irqBasic: init\n");

    if (irq < 0)
        return -EINVAL;

    ret = request_irq(
        irq,
        irq_handler,
        0,
        "ldd_irq_basic",
        &irq
    );

    if (ret)
    {
        printk(KERN_ERR
            "request_irq() failed\n");

        return ret;
    }

    printk(KERN_INFO
        "IRQ %d registered\n",
        irq);

    return 0;
}

//-------------------------------------------

static void __exit irq_basic_exit(void)
{
    free_irq(irq, &irq);

    printk(KERN_INFO
        "ldd_20261006_irqBasic: exit\n");
}

//-------------------------------------------

module_init(irq_basic_init);
module_exit(irq_basic_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic IRQ demonstration");

//-------------------------------------------


