#include <linux/module.h>
#include <linux/blk-mq.h>
#include <linux/blkdev.h>
#include <linux/vmalloc.h>
#include <linux/highmem.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("Virtual block device");

//-------------------------------------------

#define LDD_NAME "ldd_virtualBlockDevice"

#define LDD_SECTOR_SIZE 512
#define LDD_SECTORS     16384

//-------------------------------------------

struct ldd_virtual_device {
    void* storage;

    struct gendisk* disk;
    struct blk_mq_tag_set tag_set;

    int major;
};

//-------------------------------------------

static struct ldd_virtual_device* ldd_dev;

//-------------------------------------------

static blk_status_t ldd_transfer(
    struct request* rq)
{
    struct ldd_virtual_device* dev =
        ldd_dev;

    struct bio_vec bvec;
    struct req_iterator iter;

    sector_t sector = blk_rq_pos(rq);

    bool write = req_op(rq) == REQ_OP_WRITE;

    //-------------------------------------------

    rq_for_each_segment(bvec, rq, iter) {

        void* buffer;
        void* storage;
        unsigned int len;

        len = bvec.bv_len;

        storage =
            dev->storage +
            sector * LDD_SECTOR_SIZE;

        //---------------------------------------

        if (sector * LDD_SECTOR_SIZE + len >
            LDD_SECTORS * LDD_SECTOR_SIZE)
            return BLK_STS_IOERR;

        //---------------------------------------

        buffer = kmap_local_page(bvec.bv_page);
        buffer += bvec.bv_offset;

        //---------------------------------------

        if (write)
            memcpy(storage, buffer, len);
        else
            memcpy(buffer, storage, len);

        //---------------------------------------

        kunmap_local(buffer - bvec.bv_offset);

        sector += len / LDD_SECTOR_SIZE;
    }

    return BLK_STS_OK;
}

//-------------------------------------------

static blk_status_t ldd_queue_rq(
    struct blk_mq_hw_ctx* hctx,
    const struct blk_mq_queue_data* bd)
{
    struct request* rq = bd->rq;

    blk_mq_start_request(rq);

    blk_mq_end_request(
        rq,
        ldd_transfer(rq));

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

    ldd_dev = kzalloc(sizeof(*ldd_dev), GFP_KERNEL);

    if (!ldd_dev)
        return -ENOMEM;

    //-------------------------------------------

    ldd_dev->storage =
        vzalloc(LDD_SECTOR_SIZE * LDD_SECTORS);

    if (!ldd_dev->storage) {
        ret = -ENOMEM;
        goto free_dev;
    }

    //-------------------------------------------

    ldd_dev->major =
        register_blkdev(0, LDD_NAME);

    if (ldd_dev->major < 0) {
        ret = ldd_dev->major;
        goto free_storage;
    }

    //-------------------------------------------

    ldd_dev->tag_set.ops = &ldd_mq_ops;
    ldd_dev->tag_set.nr_hw_queues = 1;
    ldd_dev->tag_set.queue_depth = 32;
    ldd_dev->tag_set.numa_node = NUMA_NO_NODE;

    ret = blk_mq_alloc_tag_set(&ldd_dev->tag_set);

    if (ret)
        goto unregister_major;

    //-------------------------------------------

    ldd_dev->disk =
        blk_mq_alloc_disk(&ldd_dev->tag_set,
            ldd_dev);

    if (IS_ERR(ldd_dev->disk)) {
        ret = PTR_ERR(ldd_dev->disk);
        goto free_tagset;
    }

    //-------------------------------------------

    ldd_dev->disk->major = ldd_dev->major;
    ldd_dev->disk->first_minor = 0;
    ldd_dev->disk->minors = 1;

    strscpy(ldd_dev->disk->disk_name,
        LDD_NAME,
        DISK_NAME_LEN);

    ldd_dev->disk->fops = &ldd_fops;

    //-------------------------------------------

    set_capacity(ldd_dev->disk, LDD_SECTORS);

    //-------------------------------------------

    ret = add_disk(ldd_dev->disk);

    if (ret)
        goto put_disk;

    //-------------------------------------------

    pr_info("ldd_virtualBlockDevice loaded\n");

    return 0;

    //-------------------------------------------

put_disk:

    put_disk(ldd_dev->disk);

free_tagset:

    blk_mq_free_tag_set(&ldd_dev->tag_set);

unregister_major:

    unregister_blkdev(ldd_dev->major, LDD_NAME);

free_storage:

    vfree(ldd_dev->storage);

free_dev:

    kfree(ldd_dev);

    return ret;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    del_gendisk(ldd_dev->disk);
    put_disk(ldd_dev->disk);

    blk_mq_free_tag_set(&ldd_dev->tag_set);

    unregister_blkdev(ldd_dev->major,
        LDD_NAME);

    vfree(ldd_dev->storage);

    kfree(ldd_dev);

    pr_info("ldd_virtualBlockDevice removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------


/*
//-------------------------------------------


//-------------------------------------------
*/


