#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mychardev"
#define BUF_LEN 128

static int major;
static char msg[BUF_LEN];
static int msg_len = 0;

static int dev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device opened\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *filp, char *buffer, size_t len, loff_t *offset)
{
    int bytes_read = 0;
    if (*offset >= msg_len) return 0;

    while (len && *offset < msg_len) {
        put_user(msg[*offset], buffer++);
        len--;
        (*offset)++;
        bytes_read++;
    }

    printk(KERN_INFO "Read %d bytes from device\n", bytes_read);
    return bytes_read;
}

static ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    int i;
    for (i = 0; i < len && i < BUF_LEN; i++) {
        get_user(msg[i], buff + i);
    }
    msg_len = i;
    printk(KERN_INFO "Received %d bytes\n", msg_len);
    return msg_len;
}

static struct file_operations fops = {
    .read = dev_read,
    .write = dev_write,
    .open = dev_open,
    .release = dev_release,
};

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", major);
        return major;
    }

    printk(KERN_INFO "Char device registered with major number %d\n", major);
    return 0;
}

static void __exit chardev_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Char device unregistered\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("You");
MODULE_DESCRIPTION("Simple Character Device Driver");

