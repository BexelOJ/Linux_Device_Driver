//-------------------------------------------
// ldd_20261006_sharedIRQ.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int irq = -1;

module_param(irq, int, 0444);
MODULE_PARM_DESC(irq, "IRQ number");

//-------------------------------------------

static int device1;
static int device2;

//-------------------------------------------

static irqreturn_t device1_handler(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "sharedIRQ: device1 handler called\n");

    return IRQ_HANDLED;
}

//-------------------------------------------

static irqreturn_t device2_handler(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "sharedIRQ: device2 handler called\n");

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init shared_irq_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_sharedIRQ: init\n");

    if (irq < 0)
    {
        printk(KERN_ERR
            "Provide IRQ using irq=<number>\n");

        return -EINVAL;
    }

    ret = request_irq(
        irq,
        device1_handler,
        IRQF_SHARED,
        "ldd_shared_device1",
        &device1
    );

    if (ret)
    {
        printk(KERN_ERR
            "Failed to request IRQ for device1\n");

        return ret;
    }

    ret = request_irq(
        irq,
        device2_handler,
        IRQF_SHARED,
        "ldd_shared_device2",
        &device2
    );

    if (ret)
    {
        free_irq(irq, &device1);

        return ret;
    }

    printk(KERN_INFO
        "Shared IRQ %d registered\n",
        irq);

    return 0;
}

//-------------------------------------------

static void __exit shared_irq_exit(void)
{
    free_irq(irq, &device2);
    free_irq(irq, &device1);

    printk(KERN_INFO
        "ldd_20261006_sharedIRQ: exit\n");
}

//-------------------------------------------

module_init(shared_irq_init);
module_exit(shared_irq_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lr");
MODULE_DESCRIPTION("Shared IRQ demonstration");

//-------------------------------------------


