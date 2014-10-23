#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/hrtimer.h>
#include <linux/ktime.h>

#include <linux/kdev_t.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

unsigned long timer_interval_ns = 1e6;
static struct hrtimer hr_timer;
unsigned int value = 0;

dev_t dev;

enum hrtimer_restart timer_callback(struct hrtimer *timer_for_restart) {
	ktime_t cur_time, interval;
	
	//hr_timer restart
	cur_time = ktime_get();
	interval = ktime_set(0, timer_interval_ns);
	hrtimer_forward(timer_for_restart, cur_time, interval);
	
	//inc value
	//printk(KERN_ALERT "value = %d\n", value);
	value++;
	
	return HRTIMER_RESTART;
}

static int __init timer_init(void) {
	int err;
	ktime_t interval;
	
	//register dev
	err = alloc_chrdev_region(&dev, 0, 1, "sd_timer");
	if (err) {
		printk(KERN_ALERT "Cannot register device. Module terminating\n");
		return -1;
	}
	
	//hr_timer init
	interval = ktime_set(0, timer_interval_ns);
	hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hr_timer.function = &timer_callback;
	hrtimer_start(&hr_timer, interval, HRTIMER_MODE_REL);
	
	
	printk(KERN_ALERT "Module init done\n");
	return 0;
}

static void __exit timer_exit(void) {
	int err;
	
	//unregister dev
	unregister_chrdev_region(dev, 1);
	
	//hr_timer stop
	err = hrtimer_cancel(&hr_timer);
	if (err)
		printk(KERN_ALERT "Cannot cancel timer, it still in use\n");
	
	//work done
	printk(KERN_ALERT "Module terminated\n");
}

module_init(timer_init);
module_exit(timer_exit);