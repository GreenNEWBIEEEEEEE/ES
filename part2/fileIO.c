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
    char buf[300];
    loff_t pos;
    memset(buf, 0, sizeof(buf));

    filp = file_open("./input.txt", O_RDONLY, 0644);

    pos = 0;
    kernel_read(filp, buf, sizeof(buf), &pos);

    filp_close(filp, NULL);

    reverseString(buf);

    filp = file_open("./output.txt", O_WRONLY | O_CREAT, 0644);

    pos = 0;
    kernel_write(filp, buf, strlen(buf) * sizeof(char), &pos);

    filp_close(filp, NULL);

    return 0;
}


static void __exit hello_exit(void)
{
}

module_init(hello_init);
module_exit(hello_exit);
