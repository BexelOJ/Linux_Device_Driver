//-------------------------------------------
// ldd_20261006_workqueueIRQ.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

static int irq = -1;

module_param(irq, int, 0444);

static struct work_struct irqWork;

//-------------------------------------------

static void irq_work_function(
    struct work_struct* work)
{
    printk(KERN_INFO
        "workqueueIRQ: deferred work executed\n");
}

//-------------------------------------------

static irqreturn_t irq_handler(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "workqueueIRQ: interrupt received\n");

    schedule_work(&irqWork);

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init workqueue_irq_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_workqueueIRQ: init\n");

    if (irq < 0)
        return -EINVAL;

    INIT_WORK(
        &irqWork,
        irq_work_function
    );

    ret = request_irq(
        irq,
        irq_handler,
        0,
        "ldd_workqueue_irq",
        &irq
    );

    if (ret)
        return ret;

    return 0;
}

//-------------------------------------------

static void __exit workqueue_irq_exit(void)
{
    free_irq(irq, &irq);

    flush_work(&irqWork);

    printk(KERN_INFO
        "ldd_20261006_workqueueIRQ: exit\n");
}

//-------------------------------------------

module_init(workqueue_irq_init);
module_exit(workqueue_irq_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("IRQ and workqueue demonstration");

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/



