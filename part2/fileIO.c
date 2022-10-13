#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_DESCRIPTION("hello_world");
MODULE_LICENSE("GPL");

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello world !\n");
    return 0;
}


static void __exit hello_exit(void)
{
    printk(KERN_INFO "Bye !\n");
}


void file_read()
{

    struct file* filp = filp_open("./input.txt", O_RDONLY, 0);
    
    vfs_read();

    filp_close(filp, NULL);

}

module_init(hello_init);
module_exit(hello_exit);
