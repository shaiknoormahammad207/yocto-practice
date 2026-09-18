#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>   /* Needed for copy_to_user, copy_from_user */
#include <linux/cdev.h>
#include <linux/device.h>

#define DEVICE_NAME "my_device"
#define CLASS_NAME  "my_class"
#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noor");
MODULE_DESCRIPTION("A Simple Character Device Driver for i.MX6ULL");

static int major_number;
static char kernel_buffer[BUFFER_SIZE];
static short size_of_message;
static struct class*  chardev_class  = NULL;
static struct device* chardev_device = NULL;

/* 1. Device Open Callback */
static int dev_open(struct inode *inodep, struct file *filep)
{
    pr_info("my_chardev: Device successfully opened\n");
    return 0;
}

/* 2. Device Read Callback (Kernel -> User) */
static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{
    int errors = 0;

    /* Check if user has already read the message */
    if (*offset >= size_of_message)
        return 0;

    /* Adjust length to avoid reading past end */
    if (len > size_of_message - *offset)
        len = size_of_message - *offset;

    /* Safely transfer data to user space */
    errors = copy_to_user(buffer, kernel_buffer + *offset, len);

    if (errors == 0) {
        pr_info("my_chardev: Sent %zu characters to user\n", len);
        *offset += len;
        return len;
    } else {
        pr_err("my_chardev: Failed to send %d characters to user\n", errors);
        return -EFAULT;
    }
}

/* 3. Device Write Callback (User -> Kernel) */
static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset)
{
    if (len > BUFFER_SIZE - 1)
        len = BUFFER_SIZE - 1;

    /* Safely receive data from user space */
    if (copy_from_user(kernel_buffer, buffer, len) != 0) {
        pr_err("my_chardev: Failed to copy data from user\n");
        return -EFAULT;
    }

    kernel_buffer[len] = '\0'; /* Null terminate */
    size_of_message = len;
    pr_info("my_chardev: Received %zu characters: \"%s\"\n", len, kernel_buffer);
    return len;
}

/* 4. Device Release/Close Callback */
static int dev_release(struct inode *inodep, struct file *filep)
{
    pr_info("my_chardev: Device successfully closed\n");
    return 0;
}

/* File operations mapping */
static struct file_operations fops =
{
    .owner   = THIS_MODULE,
    .open    = dev_open,
    .read    = dev_read,
    .write   = dev_write,
    .release = dev_release,
};

/* Module Init */
static int __init chardev_init(void)
{
    pr_info("my_chardev: Initializing driver\n");

    /* Allocate a major number dynamically */
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_err("my_chardev: Failed to register a major number\n");
        return major_number;
    }

    /* Register device class */
    chardev_class = class_create(CLASS_NAME);
    if (IS_ERR(chardev_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_err("my_chardev: Failed to register device class\n");
        return PTR_ERR(chardev_class);
    }

    /* Create the device node /dev/my_device */
    chardev_device = device_create(chardev_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(chardev_device)) {
        class_destroy(chardev_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_err("my_chardev: Failed to create device\n");
        return PTR_ERR(chardev_device);
    }

    pr_info("my_chardev: Device created successfully at /dev/%s\n", DEVICE_NAME);
    return 0;
}

/* Module Exit */
static void __exit chardev_exit(void)
{
    device_destroy(chardev_class, MKDEV(major_number, 0));
    class_unregister(chardev_class);
    class_destroy(chardev_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("my_chardev: Driver unloaded\n");
}

module_init(chardev_init);
module_exit(chardev_exit);
