#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("hello_world");
MODULE_LICENSE("GPL");

struct file* file_open(const char *filename, int flags, umode_t mode)
{
    struct file* filp = filp_open(filename, flags, mode);

    if (IS_ERR(filp))
    {
        printk(KERN_INFO "Open file failed \n");
        return NULL;
    }

    return filp;
}

void file_close(struct file* filp)
{
    filp_close(filp, NULL);
}

static int __init hello_init(void)
{
    struct file* filp;
    char buf[512] = "";
    loff_t pos;

    printk(KERN_INFO "Hello world !\n");
    //mm_segment_t old_fs = get_fs();
    //set_fs(KERNEL_DS);

    filp = file_open("./input.txt", O_RDONLY, 0644);
    
    pos = 0;
    kernel_read(filp, buf, sizeof(buf), &pos);
    
    file_close(filp);
    

    filp = file_open("./output.txt", O_WRONLY | O_CREAT, 0644);
    
    pos = 0;
    kernel_write(filp, buf, sizeof(buf), &pos);
    
    file_close(filp);
    //set_fs(old_fs);
    return 0;
}


static void __exit hello_exit(void)
{
    printk(KERN_INFO "Bye !\n");
}

module_init(hello_init);
module_exit(hello_exit);
