#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>

/*
 * PWM Device Tree consumer.
 *
 * DT:
 *
 *     pwms = <&pwm0 0 20000000 0>;
 *
 * means:
 *
 *     PWM controller
 *     channel
 *     period
 *     flags
 */

 //-------------------------------------------

static int pwm_dt_probe(struct platform_device* pdev)
{
    struct pwm_device* pwm;
    struct pwm_state state;

    int ret;

    pr_info("pwmDeviceTree: probe()\n");

    pwm = devm_pwm_get(&pdev->dev, NULL);

    if (IS_ERR(pwm))
    {
        ret = PTR_ERR(pwm);

        pr_err("pwmDeviceTree: PWM get failed: %d\n",
            ret);

        return ret;
    }

    pwm_init_state(pwm, &state);

    pr_info("pwmDeviceTree: period = %llu ns\n",
        state.period);

    state.duty_cycle = state.period / 2;
    state.enabled = true;

    ret = pwm_apply_might_sleep(pwm, &state);

    if (ret)
    {
        pr_err("pwmDeviceTree: apply failed: %d\n",
            ret);

        return ret;
    }

    platform_set_drvdata(pdev, pwm);

    return 0;
}

//-------------------------------------------

static void pwm_dt_remove(struct platform_device* pdev)
{
    struct pwm_device* pwm;
    struct pwm_state state;

    pwm = platform_get_drvdata(pdev);

    if (!pwm)
        return;

    pwm_get_state(pwm, &state);

    state.enabled = false;

    pwm_apply_might_sleep(pwm, &state);

    pr_info("pwmDeviceTree: PWM disabled\n");
}

//-------------------------------------------

static const struct of_device_id pwm_dt_of_match[] =
{
    {
        .compatible = "ldd,pwm-device-tree",
    },
    { }
};

MODULE_DEVICE_TABLE(of, pwm_dt_of_match);

//-------------------------------------------

static struct platform_driver pwm_dt_driver =
{
    .probe = pwm_dt_probe,
    .remove = pwm_dt_remove,

    .driver =
    {
        .name = "ldd_pwm_device_tree",
        .of_match_table = pwm_dt_of_match,
    },
};

//-------------------------------------------

module_platform_driver(pwm_dt_driver);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("PWM Device Tree consumer");



