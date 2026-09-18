#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "my_gpio"

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;

static struct gpio_desc *my_gpio;

/* ---------------------------------------------------------
 * File operations
 * --------------------------------------------------------- */

static int my_gpio_open(struct inode *inode, struct file *file)
{
    pr_info("my_gpio: device opened\n");
    return 0;
}

static int my_gpio_release(struct inode *inode, struct file *file)
{
    pr_info("my_gpio: device closed\n");
    return 0;
}

static ssize_t my_gpio_write(struct file *file,
                             const char __user *buf,
                             size_t count,
                             loff_t *ppos)
{
    char value;

    if (copy_from_user(&value, buf, 1))
        return -EFAULT;

    if (value == '1') {
        gpiod_set_value_cansleep(my_gpio, 1);
        pr_info("my_gpio: GPIO ON\n");
    }
    else if (value == '0') {
        gpiod_set_value_cansleep(my_gpio, 0);
        pr_info("my_gpio: GPIO OFF\n");
    }
    else {
        pr_info("my_gpio: use 1 for ON, 0 for OFF\n");
        return -EINVAL;
    }

    return count;
}

static const struct file_operations my_gpio_fops = {
    .owner   = THIS_MODULE,
    .open    = my_gpio_open,
    .write   = my_gpio_write,
    .release = my_gpio_release,
};

/* ---------------------------------------------------------
 * Platform driver probe
 * --------------------------------------------------------- */

static int my_gpio_probe(struct platform_device *pdev)
{
    int ret;

    pr_info("my_gpio: probe started\n");

    /*
     * Get GPIO from Device Tree.
     *
     * This reads:
     *
     * gpios = <&gpio1 16 GPIO_ACTIVE_HIGH>;
     *
     * from the my_gpio node.
     */
    my_gpio = devm_gpiod_get(&pdev->dev, NULL, GPIOD_OUT_LOW);

    if (IS_ERR(my_gpio)) {
        ret = PTR_ERR(my_gpio);

        pr_err("my_gpio: failed to get GPIO: %d\n", ret);

        return ret;
    }

    pr_info("my_gpio: GPIO successfully configured\n");

    /* Allocate device number */
    ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);

    if (ret) {
        pr_err("my_gpio: alloc_chrdev_region failed\n");
        return ret;
    }

    /* Initialize character device */
    cdev_init(&my_cdev, &my_gpio_fops);

    my_cdev.owner = THIS_MODULE;

    ret = cdev_add(&my_cdev, dev_num, 1);

    if (ret) {
        pr_err("my_gpio: cdev_add failed\n");
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    /* Create class */
    my_class = class_create(DEVICE_NAME);

    if (IS_ERR(my_class)) {
        ret = PTR_ERR(my_class);

        pr_err("my_gpio: class_create failed\n");

        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);

        return ret;
    }

    /* Create /dev/my_gpio */
    if (IS_ERR(device_create(my_class,
                             NULL,
                             dev_num,
                             NULL,
                             DEVICE_NAME))) {

        pr_err("my_gpio: device_create failed\n");

        class_destroy(my_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);

        return -ENOMEM;
    }

    pr_info("my_gpio: driver probed successfully\n");
    pr_info("my_gpio: device created at /dev/%s\n", DEVICE_NAME);

    return 0;
}

/* ---------------------------------------------------------
 * Platform driver remove
 * --------------------------------------------------------- */

static int my_gpio_remove(struct platform_device *pdev)
{
    pr_info("my_gpio: remove\n");

    device_destroy(my_class, dev_num);
    class_destroy(my_class);

    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);

    return 0;
}

/* ---------------------------------------------------------
 * Device Tree match table
 * --------------------------------------------------------- */

static const struct of_device_id my_gpio_of_match[] = {
    {
        .compatible = "noor,my-gpio",
    },
    { }
};

MODULE_DEVICE_TABLE(of, my_gpio_of_match);

/* ---------------------------------------------------------
 * Platform driver
 * --------------------------------------------------------- */

static struct platform_driver my_gpio_driver = {
    .probe  = my_gpio_probe,
    .remove = my_gpio_remove,

    .driver = {
        .name = DEVICE_NAME,
        .of_match_table = my_gpio_of_match,
    },
};

/* ---------------------------------------------------------
 * Module init / exit
 * --------------------------------------------------------- */

static int __init my_gpio_init(void)
{
    pr_info("my_gpio: module loaded\n");

    return platform_driver_register(&my_gpio_driver);
}

static void __exit my_gpio_exit(void)
{
    pr_info("my_gpio: module unloaded\n");

    platform_driver_unregister(&my_gpio_driver);
}

module_init(my_gpio_init);
module_exit(my_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Noor");
MODULE_DESCRIPTION("Simple GPIO platform driver for i.MX6ULL");
MODULE_VERSION("1.0");
