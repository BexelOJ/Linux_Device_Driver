#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/workqueue.h>

//-------------------------------------------

struct ldd_button_data {
    struct input_dev* input;
    struct delayed_work work;
    bool pressed;
};

static struct ldd_button_data* button_data;

//-------------------------------------------

static void ldd_button_work(struct work_struct* work)
{
    struct ldd_button_data* data;

    data = container_of(to_delayed_work(work),
        struct ldd_button_data,
        work);

    //-------------------------------------------
    // Toggle state
    //-------------------------------------------

    data->pressed = !data->pressed;

    //-------------------------------------------
    // Report button state
    //-------------------------------------------

    input_report_key(data->input,
        KEY_ENTER,
        data->pressed);

    input_sync(data->input);

    pr_info("ldd_inputButton: %s\n",
        data->pressed ? "pressed" : "released");

    //-------------------------------------------
    // Schedule next event
    //-------------------------------------------

    schedule_delayed_work(&data->work,
        msecs_to_jiffies(2000));
}

//-------------------------------------------

static int __init ldd_inputButton_init(void)
{
    int ret;

    pr_info("ldd_inputButton: init\n");

    //-------------------------------------------
    // Allocate private data
    //-------------------------------------------

    button_data = kzalloc(sizeof(*button_data),
        GFP_KERNEL);

    if (!button_data)
        return -ENOMEM;

    //-------------------------------------------
    // Allocate input device
    //-------------------------------------------

    button_data->input = input_allocate_device();

    if (!button_data->input) {
        kfree(button_data);
        return -ENOMEM;
    }

    //-------------------------------------------
    // Configure
    //-------------------------------------------

    button_data->input->name = "ldd-virtual-button";
    button_data->input->phys = "ldd/button0";
    button_data->input->id.bustype = BUS_VIRTUAL;

    input_set_capability(button_data->input,
        EV_KEY,
        KEY_ENTER);

    //-------------------------------------------
    // Register
    //-------------------------------------------

    ret = input_register_device(button_data->input);

    if (ret) {
        input_free_device(button_data->input);
        kfree(button_data);
        return ret;
    }

    //-------------------------------------------
    // Initialize work
    //-------------------------------------------

    INIT_DELAYED_WORK(&button_data->work,
        ldd_button_work);

    schedule_delayed_work(&button_data->work,
        msecs_to_jiffies(2000));

    return 0;
}

//-------------------------------------------

static void __exit ldd_inputButton_exit(void)
{
    pr_info("ldd_inputButton: exit\n");

    cancel_delayed_work_sync(&button_data->work);

    input_unregister_device(button_data->input);

    kfree(button_data);
}

//-------------------------------------------

module_init(ldd_inputButton_init);
module_exit(ldd_inputButton_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux Input Button example");


/*



*/


