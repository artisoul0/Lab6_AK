#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Veruga Yuriy");
MODULE_DESCRIPTION("Lab 6");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int countGreet = 1;

module_param(countGreet, uint, S_IRUGO);

MODULE_PARM_DESC(countGreet, " Count of greets of Hello, World");

struct event_list {
    struct list_head list;
    ktime_t timeOfEvent;
};

static struct list_head event_list_head;

static LIST_HEAD(event_list_head);

void podiya(void);

static int __init initializeHello(void)
{
    if (countGreet == 0 || (countGreet >= 5 && countGreet <= 10)) {
        printk(KERN_WARNING "The value equals 0 or stands in range between 5 and 10");  
    }

    if (countGreet > 10) {
        printk(KERN_ERR "Amount is bigger than 10");
        return -EINVAL;
    }
  
    int i;
    for (i = 0; i < countGreet; i++) {
        printk(KERN_EMERG "Hello, World!\n");
        podiya();
    }
  return 0;
}

static void __exit outOfHello(void) {
  struct event_list *md, *tmp;
  list_for_each_entry_safe(md, tmp, &event_list_head, list) {
    printk(KERN_EMERG "Time: %lld\n", md->timeOfEvent);
    list_del(&md->list);
    kfree(md);
  }
}

void podiya(void) {
    struct event_list *element = NULL;
    element = kmalloc(sizeof(struct event_list), GFP_KERNEL);
    element->timeOfEvent = ktime_get();
    list_add_tail(&element->list, &event_list_head);
}

module_init(initializeHello);
module_exit(outOfHello);
