#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>

/*
 * Basic PWM consumer.
 *
 * PWM:
 *
 *     period    -> total cycle time
 *     duty      -> ON time
 *
 * Example:
 *
 *     period = 20 ms
 *     duty   = 10 ms
 *
 * gives approximately 50% duty cycle.
 */

 //-------------------------------------------

struct pwm_basic_data
{
    struct pwm_device* pwm;
};

//-------------------------------------------

static int pwm_basic_probe(struct platform_device* pdev)
{
    struct pwm_basic_data* data;
    struct pwm_state state;

    int ret;

    pr_info("pwmBasic: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->pwm = devm_pwm_get(&pdev->dev, NULL);

    if (IS_ERR(data->pwm))
    {
        ret = PTR_ERR(data->pwm);

        pr_err("pwmBasic: devm_pwm_get() failed: %d\n",
            ret);

        return ret;
    }

    pwm_init_state(data->pwm, &state);

    state.period = 20 * NSEC_PER_MSEC;
    state.duty_cycle = 10 * NSEC_PER_MSEC;
    state.enabled = true;

    ret = pwm_apply_might_sleep(data->pwm, &state);

    if (ret)
    {
        pr_err("pwmBasic: PWM configuration failed: %d\n",
            ret);

        return ret;
    }

    platform_set_drvdata(pdev, data);

    pr_info("pwmBasic: PWM enabled\n");

    return 0;
}

//-------------------------------------------

static void pwm_basic_remove(struct platform_device* pdev)
{
    struct pwm_basic_data* data;
    struct pwm_state state;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    pwm_get_state(data->pwm, &state);

    state.enabled = false;

    pwm_apply_might_sleep(data->pwm, &state);

    pr_info("pwmBasic: PWM disabled\n");
}

//-------------------------------------------

static const struct of_device_id pwm_basic_of_match[] =
{
    {
        .compatible = "ldd,pwm-basic",
    },
    { }
};

MODULE_DEVICE_TABLE(of, pwm_basic_of_match);

//-------------------------------------------

static struct platform_driver pwm_basic_driver =
{
    .probe = pwm_basic_probe,
    .remove = pwm_basic_remove,

    .driver =
    {
        .name = "ldd_pwm_basic",
        .of_match_table = pwm_basic_of_match,
    },
};

//-------------------------------------------

module_platform_driver(pwm_basic_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic PWM consumer driver");



