#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>

/*
 * PWM driver / consumer demonstration.
 *
 * Frequency:
 *
 *     frequency = 1 / period
 *
 * Duty cycle:
 *
 *     duty / period
 */

 //-------------------------------------------

struct pwm_driver_data
{
    struct pwm_device* pwm;
};

//-------------------------------------------

static int pwm_driver_probe(struct platform_device* pdev)
{
    struct pwm_driver_data* data;
    struct pwm_state state;

    int ret;

    pr_info("pwmDriver: probe()\n");

    data = devm_kzalloc(&pdev->dev,
        sizeof(*data),
        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    data->pwm = devm_pwm_get(&pdev->dev, NULL);

    if (IS_ERR(data->pwm))
    {
        ret = PTR_ERR(data->pwm);

        pr_err("pwmDriver: PWM get failed: %d\n",
            ret);

        return ret;
    }

    pwm_init_state(data->pwm, &state);

    /*
     * 1 kHz:
     *
     * period = 1 ms
     */

    state.period = 1 * NSEC_PER_MSEC;

    /*
     * 25% duty cycle
     */

    state.duty_cycle = 250 * NSEC_PER_USEC;

    state.enabled = true;

    ret = pwm_apply_might_sleep(data->pwm, &state);

    if (ret)
    {
        pr_err("pwmDriver: PWM apply failed: %d\n",
            ret);

        return ret;
    }

    platform_set_drvdata(pdev, data);

    pr_info("pwmDriver: 1 kHz, 25%% duty cycle\n");

    return 0;
}

//-------------------------------------------

static void pwm_driver_remove(struct platform_device* pdev)
{
    struct pwm_driver_data* data;
    struct pwm_state state;

    data = platform_get_drvdata(pdev);

    if (!data)
        return;

    pwm_get_state(data->pwm, &state);

    state.enabled = false;

    pwm_apply_might_sleep(data->pwm, &state);

    pr_info("pwmDriver: PWM disabled\n");
}

//-------------------------------------------

static const struct of_device_id pwm_driver_of_match[] =
{
    {
        .compatible = "ldd,pwm-driver",
    },
    { }
};

MODULE_DEVICE_TABLE(of, pwm_driver_of_match);

//-------------------------------------------

static struct platform_driver pwm_driver =
{
    .probe = pwm_driver_probe,
    .remove = pwm_driver_remove,

    .driver =
    {
        .name = "ldd_pwm_driver",
        .of_match_table = pwm_driver_of_match,
    },
};

//-------------------------------------------

module_platform_driver(pwm_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("PWM driver demonstration");



