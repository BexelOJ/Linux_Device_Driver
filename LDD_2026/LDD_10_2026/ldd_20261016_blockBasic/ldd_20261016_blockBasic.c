#include <linux/module.h>
#include <linux/blk-mq.h>
#include <linux/blkdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Basic Linux block device example");

//-------------------------------------------

#define LDD_BLOCK_NAME "ldd_blockBasic"
#define LDD_BLOCK_SECTORS 1024

//-------------------------------------------

static int ldd_major;

static struct gendisk* ldd_disk;
static struct blk_mq_tag_set ldd_tag_set;

//-------------------------------------------

static blk_status_t ldd_blockBasic_queue_rq(
    struct blk_mq_hw_ctx* hctx,
    const struct blk_mq_queue_data* bd)
{
    struct request* rq = bd->rq;

    //-------------------------------------------

    blk_mq_start_request(rq);

    pr_info("ldd_blockBasic: request received\n");
    pr_info("sector = %llu\n",
        (unsigned long long)blk_rq_pos(rq));

    //-------------------------------------------

    blk_mq_end_request(rq, BLK_STS_OK);

    return BLK_STS_OK;
}

//-------------------------------------------

static const struct blk_mq_ops ldd_blockBasic_ops = {
    .queue_rq = ldd_blockBasic_queue_rq,
};

//-------------------------------------------

static const struct block_device_operations ldd_blockBasic_fops = {
    .owner = THIS_MODULE,
};

//-------------------------------------------

static int __init ldd_blockBasic_init(void)
{
    int ret;

    //-------------------------------------------
    // Allocate major number
    //-------------------------------------------

    ldd_major = register_blkdev(0, LDD_BLOCK_NAME);

    if (ldd_major < 0)
        return ldd_major;

    //-------------------------------------------

    ldd_tag_set.ops = &ldd_blockBasic_ops;
    ldd_tag_set.nr_hw_queues = 1;
    ldd_tag_set.queue_depth = 16;
    ldd_tag_set.numa_node = NUMA_NO_NODE;
    ldd_tag_set.cmd_size = 0;
    ldd_tag_set.flags = BLK_MQ_F_SHOULD_MERGE;

    ret = blk_mq_alloc_tag_set(&ldd_tag_set);

    if (ret)
        goto unregister_blkdev;

    //-------------------------------------------

    ldd_disk = blk_mq_alloc_disk(&ldd_tag_set, NULL);

    if (IS_ERR(ldd_disk)) {
        ret = PTR_ERR(ldd_disk);
        goto free_tag_set;
    }

    //-------------------------------------------

    ldd_disk->major = ldd_major;
    ldd_disk->first_minor = 0;
    ldd_disk->minors = 1;

    strscpy(ldd_disk->disk_name,
        LDD_BLOCK_NAME,
        DISK_NAME_LEN);

    ldd_disk->fops = &ldd_blockBasic_fops;

    //-------------------------------------------
    // 1024 sectors = 512 KB
    //-------------------------------------------

    set_capacity(ldd_disk, LDD_BLOCK_SECTORS);

    //-------------------------------------------

    ret = add_disk(ldd_disk);

    if (ret)
        goto put_disk;

    //-------------------------------------------

    pr_info("ldd_blockBasic: registered\n");
    pr_info("major = %d\n", ldd_major);

    return 0;

    //-------------------------------------------

put_disk:

    put_disk(ldd_disk);

free_tag_set:

    blk_mq_free_tag_set(&ldd_tag_set);

unregister_blkdev:

    unregister_blkdev(ldd_major, LDD_BLOCK_NAME);

    return ret;
}

//-------------------------------------------

static void __exit ldd_blockBasic_exit(void)
{
    del_gendisk(ldd_disk);
    put_disk(ldd_disk);

    blk_mq_free_tag_set(&ldd_tag_set);

    unregister_blkdev(ldd_major, LDD_BLOCK_NAME);

    pr_info("ldd_blockBasic: removed\n");
}

//-------------------------------------------

module_init(ldd_blockBasic_init);
module_exit(ldd_blockBasic_exit);

//-------------------------------------------


/*
//-------------------------------------------

Application
    ↓
Filesystem
    ↓
Block Layer
    ↓
blk-mq
    ↓
queue_rq()
    ↓
Your block driver

//-------------------------------------------
*/
