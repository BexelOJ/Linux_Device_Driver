//-------------------------------------------
// ldd_20261006_irqAffinity.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/cpumask.h>

static int irq = -1;
static int cpu = 0;

module_param(irq, int, 0444);
module_param(cpu, int, 0444);

//-------------------------------------------

static irqreturn_t irq_affinity_handler(
    int irq,
    void* dev_id)
{
    printk(KERN_INFO
        "irqAffinity: IRQ handled on CPU %d\n",
        smp_processor_id());

    return IRQ_HANDLED;
}

//-------------------------------------------

static int __init irq_affinity_init(void)
{
    int ret;

    printk(KERN_INFO
        "ldd_20261006_irqAffinity: init\n");

    if (irq < 0)
        return -EINVAL;

    if (cpu < 0 || cpu >= nr_cpu_ids)
        return -EINVAL;

    ret = request_irq(
        irq,
        irq_affinity_handler,
        0,
        "ldd_irq_affinity",
        &irq
    );

    if (ret)
        return ret;

    ret = irq_set_affinity(
        irq,
        cpumask_of(cpu)
    );

    if (ret)
    {
        free_irq(irq, &irq);

        return ret;
    }

    printk(KERN_INFO
        "IRQ %d assigned to CPU %d\n",
        irq,
        cpu);

    return 0;
}

//-------------------------------------------

static void __exit irq_affinity_exit(void)
{
    free_irq(irq, &irq);

    printk(KERN_INFO
        "ldd_20261006_irqAffinity: exit\n");
}

//-------------------------------------------

module_init(irq_affinity_init);
module_exit(irq_affinity_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("IRQ CPU affinity demonstration");

//-------------------------------------------


