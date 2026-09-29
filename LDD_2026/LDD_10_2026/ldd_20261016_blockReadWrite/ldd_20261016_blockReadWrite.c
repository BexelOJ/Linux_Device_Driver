#include <linux/module.h>
#include <linux/blk-mq.h>
#include <linux/blkdev.h>
#include <linux/vmalloc.h>
#include <linux/highmem.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Er Bexel O J");
MODULE_DESCRIPTION("RAM block device read/write example");

//-------------------------------------------

#define LDD_BLOCK_NAME "ldd_blockReadWrite"

#define LDD_SECTOR_SIZE 512
#define LDD_SECTORS     32768

#define LDD_DISK_SIZE \
    (LDD_SECTOR_SIZE * LDD_SECTORS)

//-------------------------------------------

static int ldd_major;

static struct gendisk* ldd_disk;
static struct blk_mq_tag_set ldd_tag_set;

static void* ldd_storage;

//-------------------------------------------

static blk_status_t ldd_transfer_request(struct request* rq)
{
    struct bio_vec bvec;
    struct req_iterator iter;

    sector_t sector = blk_rq_pos(rq);

    bool write = (req_op(rq) == REQ_OP_WRITE);

    //-------------------------------------------

    rq_for_each_segment(bvec, rq, iter) {

        void* buffer;
        size_t offset;
        size_t len;

        //---------------------------------------

        offset = sector * LDD_SECTOR_SIZE;
        len = bvec.bv_len;

        //---------------------------------------

        if (offset + len > LDD_DISK_SIZE)
            return BLK_STS_IOERR;

        //---------------------------------------

        buffer = kmap_local_page(bvec.bv_page);
        buffer += bvec.bv_offset;

        //---------------------------------------

        if (write) {

            memcpy(ldd_storage + offset,
                buffer,
                len);

        }
        else {

            memcpy(buffer,
                ldd_storage + offset,
                len);
        }

        //---------------------------------------

        kunmap_local(buffer - bvec.bv_offset);

        //---------------------------------------

        sector += len >> 9;
    }

    return BLK_STS_OK;
}

//-------------------------------------------

static blk_status_t ldd_queue_rq(
    struct blk_mq_hw_ctx* hctx,
    const struct blk_mq_queue_data* bd)
{
    struct request* rq = bd->rq;

    blk_status_t status;

    //-------------------------------------------

    blk_mq_start_request(rq);

    //-------------------------------------------

    status = ldd_transfer_request(rq);

    //-------------------------------------------

    blk_mq_end_request(rq, status);

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
    // Allocate RAM storage
    //-------------------------------------------

    ldd_storage = vzalloc(LDD_DISK_SIZE);

    if (!ldd_storage)
        return -ENOMEM;

    //-------------------------------------------
    // Allocate major number
    //-------------------------------------------

    ldd_major = register_blkdev(0, LDD_BLOCK_NAME);

    if (ldd_major < 0) {
        ret = ldd_major;
        goto free_storage;
    }

    //-------------------------------------------

    ldd_tag_set.ops = &ldd_mq_ops;
    ldd_tag_set.nr_hw_queues = 1;
    ldd_tag_set.queue_depth = 32;
    ldd_tag_set.numa_node = NUMA_NO_NODE;

    ret = blk_mq_alloc_tag_set(&ldd_tag_set);

    if (ret)
        goto unregister_major;

    //-------------------------------------------

    ldd_disk = blk_mq_alloc_disk(&ldd_tag_set, NULL);

    if (IS_ERR(ldd_disk)) {
        ret = PTR_ERR(ldd_disk);
        goto free_tagset;
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
    // Set capacity in 512-byte sectors
    //-------------------------------------------

    set_capacity(ldd_disk, LDD_SECTORS);

    //-------------------------------------------

    ret = add_disk(ldd_disk);

    if (ret)
        goto put_disk;

    pr_info("ldd_blockReadWrite: loaded\n");
    pr_info("Size = %u KB\n",
        LDD_DISK_SIZE / 1024);

    return 0;

    //-------------------------------------------

put_disk:

    put_disk(ldd_disk);

free_tagset:

    blk_mq_free_tag_set(&ldd_tag_set);

unregister_major:

    unregister_blkdev(ldd_major, LDD_BLOCK_NAME);

free_storage:

    vfree(ldd_storage);

    return ret;
}

//-------------------------------------------

static void __exit ldd_exit(void)
{
    del_gendisk(ldd_disk);
    put_disk(ldd_disk);

    blk_mq_free_tag_set(&ldd_tag_set);

    unregister_blkdev(ldd_major, LDD_BLOCK_NAME);

    vfree(ldd_storage);

    pr_info("ldd_blockReadWrite: removed\n");
}

//-------------------------------------------

module_init(ldd_init);
module_exit(ldd_exit);

//-------------------------------------------


/*
//-------------------------------------------



//-------------------------------------------
*/


