#include <linux/module.h>
#include <linux/blk-mq.h>
#include <linux/blkdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Block device IOCTL example");

//-------------------------------------------

#define LDD_NAME "ldd_blockIOCTL"

#define LDD_IOCTL_MAGIC 'L'

#define LDD_IOCTL_GET_SECTORS \
    _IOR(LDD_IOCTL_MAGIC, 1, unsigned long long)

#define LDD_IOCTL_GET_SECTOR_SIZE \
    _IOR(LDD_IOCTL_MAGIC, 2, unsigned int)

//-------------------------------------------

#define LDD_SECTORS 4096
#define LDD_SECTOR_SIZE 512

//-------------------------------------------

static int ldd_major;
static struct gendisk* ldd_disk;
static struct blk_mq_tag_set ldd_tag_set;

//-------------------------------------------

static long ldd_ioctl(
    struct file* file,
    unsigned int cmd,
    unsigned long arg)
{
    unsigned long long sectors;
    unsigned int sector_size;

    //-------------------------------------------

    switch (cmd) {

    case LDD_IOCTL_GET_SECTORS:

        sectors = LDD_SECTORS;

        if (copy_to_user(
            (void __user*)arg,
            &sectors,
            sizeof(sectors)))
            return -EFAULT;

        break;

        //-------------------------------------------

    case LDD_IOCTL_GET_SECTOR_SIZE:

        sector_size = LDD_SECTOR_SIZE;

        if (copy_to_user(
            (void __user*)arg,
            &sector_size,
            sizeof(sector_size)))
            return -EFAULT;

        break;

        //-------------------------------------------

    default:

        return -ENOTTY;
    }

    return 0;
}

//-------------------------------------------

static blk_status_t ldd_queue_rq(
    struct blk_mq_hw_ctx* hctx,
    const struct blk_mq_queue_data* bd)
{
    struct request* rq = bd->rq;

    blk_mq_start_request(rq);

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

    .unlocked_ioctl = ldd_ioctl,
};

//-------------------------------------------

static int __init ldd_init(void)
{
    int ret;

    ldd_major =
        register_blkdev(0, LDD_NAME);

    if (ldd_major < 0)
        return ldd_major;

    //-------------------------------------------

    ldd_tag_set.ops = &ldd_mq_ops;
    ldd_tag_set.nr_hw_queues = 1;
    ldd_tag_set.queue_depth = 16;
    ldd_tag_set.numa_node = NUMA_NO_NODE;

    ret = blk_mq_alloc_tag_set(&ldd_tag_set);

    if (ret)
        goto error_major;

    //-------------------------------------------

    ldd_disk =
        blk_mq_alloc_disk(&ldd_tag_set, NULL);

    if (IS_ERR(ldd_disk)) {
        ret = PTR_ERR(ldd_disk);
        goto error_tagset;
    }

    //-------------------------------------------

    ldd_disk->major = ldd_major;
    ldd_disk->first_minor = 0;
    ldd_disk->minors = 1;

    strscpy(ldd_disk->disk_name,
        LDD_NAME,
        DISK_NAME_LEN);

    ldd_disk->fops = &ldd_fops;

    set_capacity(ldd_disk, LDD_SECTORS);

    //-------------------------------------------

    ret = add_disk(ldd_disk);

    if (ret)
        goto error_disk;

    pr_info("ldd_blockIOCTL loaded\n");

    return 0;

    //-------------------------------------------

error_disk:

    put_disk(ldd_disk);

error_tagset:

    blk_mq_free_tag_set(&ldd_tag_set);

error_major:

    unregister_blkdev(ldd_major, LDD_NAME);

    return ret;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    del_gendisk(ldd_disk);
    put_disk(ldd_disk);

    blk_mq_free_tag_set(&ldd_tag_set);

    unregister_blkdev(ldd_major, LDD_NAME);

    pr_info("ldd_blockIOCTL removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------

/*
//-------------------------------------------



//-------------------------------------------
*/


