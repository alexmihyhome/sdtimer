#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

unsigned long timer_interval_ns = 1e6;
static struct hrtimer hr_timer;

enum hrtimer_restart timer_callback(struct hrtimer *timer_for_restart) {
	ktime_t cur_time, interval;
	cur_time = ktime_get();
	interval = ktime_set(0, timer_interval_ns);
	hrtimer_forward(timer_for_restart, cur_time, interval);
	
	return HRTIMER_RESTART;
}

static int __init timer_init(void) {
	ktime = ktime_set(0, timer_interval_ns);
	hrtimer_init(&hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hr_timer.function = &timer_callback;
	hrtimer_start(&hr_timer, ktime, HRTIMER_MODE_REL);
	return 0;
}

static void __exit timer_exit(void) {
	int err;
	err = hrtimer_cancel(&hr_timer);
	if (err)
		printk("Cannot cancel timer, it still in use\n");
	printk("Module exited\n");
}

module_init(timer_init);
module_exit(timer_exit);