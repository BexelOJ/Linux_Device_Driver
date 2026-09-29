//-------------------------------------------
// ldd_20261004_slabCache.c
//-------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

struct my_object
{
    int id;
    char name[32];
};

static struct kmem_cache* myCache;
static struct my_object* object;

//-------------------------------------------

static int __init slab_cache_init(void)
{
    printk(KERN_INFO
        "ldd_20261004_slabCache: init\n");

    //-------------------------------------------
    // Create cache
    //-------------------------------------------

    myCache = kmem_cache_create(
        "ldd_my_cache",
        sizeof(struct my_object),
        0,
        SLAB_HWCACHE_ALIGN,
        NULL
    );

    if (!myCache)
    {
        printk(KERN_ERR
            "kmem_cache_create() failed\n");

        return -ENOMEM;
    }

    //-------------------------------------------
    // Allocate object from cache
    //-------------------------------------------

    object = kmem_cache_alloc(
        myCache,
        GFP_KERNEL
    );

    if (!object)
    {
        printk(KERN_ERR
            "kmem_cache_alloc() failed\n");

        kmem_cache_destroy(myCache);
        myCache = NULL;

        return -ENOMEM;
    }

    object->id = 100;

    snprintf(
        object->name,
        sizeof(object->name),
        "LDD Object"
    );

    printk(KERN_INFO
        "Object allocated from slab cache\n");

    printk(KERN_INFO
        "Address: %px\n",
        object);

    printk(KERN_INFO
        "id: %d\n",
        object->id);

    printk(KERN_INFO
        "name: %s\n",
        object->name);

    return 0;
}

//-------------------------------------------

static void __exit slab_cache_exit(void)
{
    if (object)
    {
        kmem_cache_free(
            myCache,
            object
        );

        object = NULL;

        printk(KERN_INFO
            "Object returned to cache\n");
    }

    if (myCache)
    {
        kmem_cache_destroy(myCache);

        myCache = NULL;

        printk(KERN_INFO
            "Slab cache destroyed\n");
    }

    printk(KERN_INFO
        "ldd_20261004_slabCache: exit\n");
}

//-------------------------------------------

module_init(slab_cache_init);
module_exit(slab_cache_exit);

//-------------------------------------------

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("SLAB/SLUB cache demonstration");

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


