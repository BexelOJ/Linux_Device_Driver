#include <linux/module.h>
#include <linux/blk-mq.h>
#include <linux/blkdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Block device registration example");

//-------------------------------------------

#define LDD_BLOCK_NAME "ldd_blockRegistration"

static int ldd_major;
static struct gendisk* ldd_disk;
static struct blk_mq_tag_set ldd_tag_set;

//-------------------------------------------

static blk_status_t ldd_queue_rq(
    struct blk_mq_hw_ctx* hctx,
    const struct blk_mq_queue_data* bd)
{
    struct request* rq = bd->rq;

    blk_mq_start_request(rq);

    pr_info("ldd_blockRegistration: request\n");

    blk_mq_end_request(rq, BLK_STS_OK);

    return BLK_STS_OK;
}

//-------------------------------------------

static const struct blk_mq_ops ldd_mq_ops = {
    .queue_rq = ldd_queue_rq,
};

//-------------------------------------------

static const struct block_device_operations ldd_fops = {
    .owner = THIS_MODULE,
};

//-------------------------------------------

static int __init ldd_init(void)
{
    int ret;

    //-------------------------------------------
    // Step 1: register major
    //-------------------------------------------

    ldd_major = register_blkdev(0, LDD_BLOCK_NAME);

    if (ldd_major < 0)
        return ldd_major;

    //-------------------------------------------
    // Step 2: initialize tag set
    //-------------------------------------------

    ldd_tag_set.ops = &ldd_mq_ops;
    ldd_tag_set.nr_hw_queues = 1;
    ldd_tag_set.queue_depth = 32;
    ldd_tag_set.numa_node = NUMA_NO_NODE;

    ret = blk_mq_alloc_tag_set(&ldd_tag_set);

    if (ret)
        goto error_major;

    //-------------------------------------------
    // Step 3: allocate gendisk
    //-------------------------------------------

    ldd_disk = blk_mq_alloc_disk(&ldd_tag_set, NULL);

    if (IS_ERR(ldd_disk)) {
        ret = PTR_ERR(ldd_disk);
        goto error_tagset;
    }

    //-------------------------------------------

    ldd_disk->major = ldd_major;
    ldd_disk->first_minor = 0;
    ldd_disk->minors = 1;

    strscpy(ldd_disk->disk_name,
        LDD_BLOCK_NAME,
        DISK_NAME_LEN);

    ldd_disk->fops = &ldd_fops;

    //-------------------------------------------
    // Step 4: capacity
    //-------------------------------------------

    set_capacity(ldd_disk, 2048);

    //-------------------------------------------
    // Step 5: expose disk
    //-------------------------------------------

    ret = add_disk(ldd_disk);

    if (ret)
        goto error_disk;

    pr_info("ldd_blockRegistration: disk registered\n");

    return 0;

    //-------------------------------------------

error_disk:

    put_disk(ldd_disk);

error_tagset:

    blk_mq_free_tag_set(&ldd_tag_set);

error_major:

    unregister_blkdev(ldd_major, LDD_BLOCK_NAME);

    return ret;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    del_gendisk(ldd_disk);
    put_disk(ldd_disk);

    blk_mq_free_tag_set(&ldd_tag_set);

    unregister_blkdev(ldd_major, LDD_BLOCK_NAME);

    pr_info("ldd_blockRegistration: removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


