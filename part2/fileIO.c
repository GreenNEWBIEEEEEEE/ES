#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>

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

void reverseString(char *s)
{
    size_t length = strlen(s);
    size_t i = 0;
    char t;
    while (i < (length >> 1))
    {
        t = s[i];
        s[i] = s[length - i - 1];
        s[length - i - 1] = t;
        i++;
    }
}

static int __init hello_init(void)
{
    struct file* filp;
    char buf[512] = "";
    loff_t pos;

    //mm_segment_t old_fs = get_fs();
    //set_fs(KERNEL_DS);

    filp = file_open("./input.txt", O_RDONLY, 0644);
    
    pos = 0;
    kernel_read(filp, buf, sizeof(buf), &pos);
    
    filp_close(filp, NULL);
    
    printk("origin: %s\n", buf);
    reverseString(buf);
    printk("reversed: %s\n", buf);
    
    filp = file_open("./output.txt", O_WRONLY | O_CREAT, 0644);
    
    pos = 0;
    kernel_write(filp, buf, sizeof(buf), &pos);
    
    filp_close(filp, NULL);
    //set_fs(old_fs);
    return 0;
}


static void __exit hello_exit(void)
{
}

module_init(hello_init);
module_exit(hello_exit);
