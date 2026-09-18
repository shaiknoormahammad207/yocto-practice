#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello from my first device driver!\n");

    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye from my first device driver!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noor");
MODULE_DESCRIPTION("My first Linux kernel module");
