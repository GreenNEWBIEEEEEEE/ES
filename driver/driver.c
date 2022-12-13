#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/device/class.h>

MODULE_DESCRIPTION("hello_world");
MODULE_LICENSE("GPL");

static dev_t my_dev;
static struct cdev my_cdev;
static int BUF_SIZE = 1024;
static char BUF[1024] = "";
static char DEVICE_NAME[] = "demo";
static char CLASS_NAME[] = "demoClass";
static struct class* my_class;


static ssize_t my_read(struct file*, char*, size_t, loff_t*);
static ssize_t my_write(struct file*, const char*, size_t, loff_t*);
static int my_open(struct inode *, struct file *);
static int my_release(struct inode*, struct file*);


// Declare a file_operations, and map them with my custom fuction.
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
    

    // Copy BUF string from kernel space to buf string from user space.
    // Return 0 if success
    if (copy_to_user(buf, BUF, length))
    {
        printk(KERN_INFO "copy to user failed\n");
        return -EFAULT;
    }

    printk(KERN_INFO "Reading String=%s\n", BUF);

    return length;
}

static ssize_t my_write(struct file* filp, const char __user* buf, size_t count, loff_t* pos)
{
    // Get length of string from user space.
    size_t length = strnlen_user(buf, BUF_SIZE);
    
    // Copy buf string from user space to BUF string in kernel space.
    // Return 0 if success
    if (copy_from_user(BUF, buf, length))
    {
        printk(KERN_INFO "copy from user failed\n");
        return -EFAULT;
    }
    
    printk(KERN_INFO "Writing String=%s\n", BUF);

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


// 1a. (when you have the major number for cdev)                 register_chrdev_region(): Register a major number into `dev_t` structure
// 1b. (when you are asking the VFS to give a free major number) alloc_chrdev_region(): Register a major number into `dev_t` structure
// 2. class_create: Create a `class` strcture.
// 3. device_create() to create device file under /dev directory.
// 
static int __init hello_init(void)
{
    int count = 1;

    // Register a range of device numbers
    // int ret = register_chrdev_region(my_dev, count, DEVICE_NAME); // Register a device number named `demo`.
    
    // if (ret < 0)
    //    printk(KERN_INFO "char device register failed %d\n", ret);

    // Register a range of char device numbers
    int ret = alloc_chrdev_region(&my_dev, 0, 1, DEVICE_NAME);

    if (ret < 0)
        printk(KERN_INFO "char device alloc chrdev region failed\n");

    // Create a struct class structure
    my_class = class_create(THIS_MODULE, CLASS_NAME);

    // Create a device node for a character device
    device_create(my_class, NULL, my_dev, NULL, DEVICE_NAME);
    
    // Initialize a `cdev` structure with my file operations.
    cdev_init(&my_cdev, &my_operations);
    
    // Add char device into system to make it be accessed by user program.
    ret = cdev_add(&my_cdev, my_dev, count);
    if (ret < 0)
        printk(KERN_INFO "char device add failed\n");

    printk(KERN_INFO "char device loaded\n");
    return 0;
}


static void __exit hello_exit(void)
{
    int count = 1;
    
    // Remove a device that was created with `device_create`
    device_destroy(my_class, my_dev);

    // Destroy a `class` structure
    class_destroy(my_class);

    // Remove a cdev from the system
    cdev_del(&my_cdev);

    unregister_chrdev_region(my_dev, count);
    
    printk(KERN_INFO "char device unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
