//-------------------------------------------
// ldd_20261006_requestIRQ.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int irq = -1;

module_param(irq, int, 0444);

//-------------------------------------------

static irqreturn_t request_irq_handler(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "requestIRQ: handler executed\n");

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init request_irq_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_requestIRQ: init\n");

    if (irq < 0)
    {
        printk(KERN_ERR
            "Use irq=<number>\n");

        return -EINVAL;
    }

    ret = request_irq(
        irq,
        request_irq_handler,
        0,
        "ldd_request_irq",
        &irq
    );

    if (ret)
    {
        printk(KERN_ERR
            "request_irq() failed: %d\n",
            ret);

        return ret;
    }

    printk(KERN_INFO
        "request_irq() successful\n");

    return 0;
}

//-------------------------------------------

static void __exit request_irq_exit(void)
{
    free_irq(irq, &irq);

    printk(KERN_INFO
        "IRQ released\n");
}

//-------------------------------------------

module_init(request_irq_init);
module_exit(request_irq_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("request_irq() demonstration");

//-------------------------------------------


