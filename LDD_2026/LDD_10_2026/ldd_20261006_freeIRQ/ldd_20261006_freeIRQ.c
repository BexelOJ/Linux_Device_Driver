//-------------------------------------------
// ldd_20261006_freeIRQ.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int irq = -1;

module_param(irq, int, 0444);

//-------------------------------------------

static irqreturn_t free_irq_handler(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "freeIRQ: handler executed\n");

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init free_irq_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_freeIRQ: init\n");

    if (irq < 0)
        return -EINVAL;

    ret = request_irq(
        irq,
        free_irq_handler,
        0,
        "ldd_free_irq",
        &irq
    );

    if (ret)
    {
        printk(KERN_ERR
            "request_irq() failed\n");

        return ret;
    }

    printk(KERN_INFO
        "IRQ %d requested\n",
        irq);

    return 0;
}

//-------------------------------------------

static void __exit free_irq_exit(void)
{
    /*
     * Stop receiving interrupts and release
     * the IRQ resource.
     */

    free_irq(irq, &irq);

    printk(KERN_INFO
        "IRQ %d freed\n",
        irq);

    printk(KERN_INFO
        "ldd_20261006_freeIRQ: exit\n");
}

//-------------------------------------------

module_init(free_irq_init);
module_exit(free_irq_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("free_irq() demonstration");

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/




