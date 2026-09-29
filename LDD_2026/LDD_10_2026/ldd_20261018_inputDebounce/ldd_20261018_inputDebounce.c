#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/workqueue.h>

//-------------------------------------------

struct ldd_debounce_data {
    struct input_dev* input;
    struct delayed_work work;
    bool state;
};

static struct ldd_debounce_data* data;

//-------------------------------------------

static void ldd_debounce_work(struct work_struct* work)
{
    struct ldd_debounce_data* button;

    button = container_of(to_delayed_work(work),
        struct ldd_debounce_data,
        work);

    //-------------------------------------------
    // Simulated stable state
    //-------------------------------------------

    button->state = !button->state;

    //-------------------------------------------
    // Report stable state
    //-------------------------------------------

    input_report_key(button->input,
        KEY_SPACE,
        button->state);

    input_sync(button->input);

    pr_info("ldd_inputDebounce: stable state = %d\n",
        button->state);
}

//-------------------------------------------

static int __init ldd_inputDebounce_init(void)
{
    int ret;

    pr_info("ldd_inputDebounce: init\n");

    //-------------------------------------------
    // Allocate private data
    //-------------------------------------------

    data = kzalloc(sizeof(*data), GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    //-------------------------------------------
    // Allocate input device
    //-------------------------------------------

    data->input = input_allocate_device();

    if (!data->input) {
        kfree(data);
        return -ENOMEM;
    }

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    data->input->name = "ldd-debounce-button";
    data->input->phys = "ldd/debounce0";
    data->input->id.bustype = BUS_VIRTUAL;

    input_set_capability(data->input,
        EV_KEY,
        KEY_SPACE);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = input_register_device(data->input);

    if (ret) {
        input_free_device(data->input);
        kfree(data);
        return ret;
    }

    //-------------------------------------------
    // Initialize debounce work
    //-------------------------------------------

    INIT_DELAYED_WORK(&data->work,
        ldd_debounce_work);

    //-------------------------------------------
    // Simulate debounce delay
    //-------------------------------------------

    schedule_delayed_work(&data->work,
        msecs_to_jiffies(50));

    return 0;
}

//-------------------------------------------

static void __exit ldd_inputDebounce_exit(void)
{
    pr_info("ldd_inputDebounce: exit\n");

    cancel_delayed_work_sync(&data->work);

    input_unregister_device(data->input);

    kfree(data);
}

//-------------------------------------------

module_init(ldd_inputDebounce_init);
module_exit(ldd_inputDebounce_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Input button debounce example");


/*
//-------------------------------------------
The real debounce sequence is:


GPIO interrupt
      │
      ▼
IRQ handler
      │
      ▼
schedule delayed work
      │
      │ 20–50 ms
      ▼
read GPIO again
      │
      ▼
stable?
      │
      ▼
input_report_key()
//-------------------------------------------
*/


