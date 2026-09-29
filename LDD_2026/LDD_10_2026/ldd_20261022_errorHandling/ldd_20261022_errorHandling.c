#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

//-------------------------------------------

static int __init error_handling_init(void)
{
    void* buffer;
    int ret = 0;

    pr_info("errorHandling: init()\n");

    buffer = kmalloc(1024, GFP_KERNEL);

    if (!buffer)
    {
        pr_err("errorHandling: kmalloc failed\n");

        ret = -ENOMEM;
        goto error_buffer;
    }

    pr_info("errorHandling: buffer allocated\n");

    /*
     * More initialization would happen here.
     */

    kfree(buffer);

    return 0;

    //-------------------------------------------

error_buffer:

    pr_err("errorHandling: initialization failed\n");

    return ret;
}

//-------------------------------------------

static void __exit error_handling_exit(void)
{
    pr_info("errorHandling: exit()\n");
}

//-------------------------------------------

module_init(error_handling_init);
module_exit(error_handling_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Linux kernel error handling example");


/*
//-------------------------------------------

resource A
   │
   ▼
resource B
   │
   ▼
resource C
   │
 failure
   │
   ▼
cleanup C
   │
   ▼
cleanup B
   │
   ▼
cleanup A

//-------------------------------------------
*/


