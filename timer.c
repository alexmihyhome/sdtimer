#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>

unsigned long timer_interval_ns = 1e6
static struct hrtimer hr_timer;



module_init(timer_init);
module_exit(timer_exit);