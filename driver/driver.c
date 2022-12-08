#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/cdev.h>

MODULE_DESCRIPTION("hello_world");
MODULE_LICENSE("GPL");

static dev_t my_dev = MKDEV(202,128);
static struct cdev my_cdev;
static int BUF_SIZE = 1024;
static char BUF[1024] = "";

static ssize_t my_read(struct file*, char*, size_t, loff_t*);
static ssize_t my_write(struct file*, const char*, size_t, loff_t*);
static int my_open(struct inode *, struct file *);
static int my_release(struct inode*, struct file*);

static struct file_operations my_operations = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_release
};



static ssize_t my_read(struct file* , char __user* buf, size_t, loff_t*)
{
    size_t length = strlen(BUF);
    
    if (copy_to_user(buf, BUF, length))
    {
        printk(KERN_INFO "copy to user failed\n");
        return -EFAULT;
    }

    return length;
}

static ssize_t my_write(struct file* filp, const char __user* buf, size_t count, loff_t* pos)
{
    size_t length = strnlen_user(BUF, BUF_SIZE);

    if (copy_from_user(BUF, buf, length))
    {
        printk(KERN_INFO "copy from user failed\n");
        return -EFAULT;
    }

    return length;
}

static int my_open(struct inode* , struct file *)
{
    printk(KERN_INFO "open\n");

    return 0;
}

static int my_release(struct inode*, struct file*)
{
    printk(KERN_INFO "release\n");
    
    return 0;
}

static int __init hello_init(void)
{
    int count = 1;
    int ret = register_chrdev_region(my_dev, count, "HW_DRIVER"); 
    if (ret < 0)
        printk(KERN_INFO "char device init failed\n");

    cdev_init(&my_cdev, &my_operations);
    
    ret = cdev_add(&my_cdev, my_dev, count);
    if (ret < 0)
        printk(KERN_INFO "char device add failed\n");

    return 0;
}


static void __exit hello_exit(void)
{
    int count = 1;
    cdev_del(&my_cdev);
    unregister_chrdev_region(my_dev, count);
}

module_init(hello_init);
module_exit(hello_exit);
