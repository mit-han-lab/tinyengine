/*
 * timer_sched.h
 *
 * LICENSE: MIT
 *
 * The following example creates two different timers that each one can host different
 * timer objects. To create a new timer object use this in your main code:
 *
 * 	// Example led structure
 * 	struct led_t {
 *  	uint16_t tick_ms;
 * 		uint8_ pattern;
 *  };
 *
 * 	// Instances of leds
 * 	struct led_t led2 = {100, 0b00110011};
 * 	struct led_t led3 = {500, 0b00110011};
 * 	struct led_t led4 = {1000, 0b00110011};
 * 	struct led_t led5 = {2000, 0b00110011};
 *
 * 	// Callback function that handles all leds in this case (it could be different)
 * 	void led_update(truct led_t * led)
 * 	{
 * 		// Handle led
 * 	}
 *
 * 	// In your main
 * 	mod_timer_add((void*) &led1, led1.tick_ms, (void*) &led_update, &timer1_list);
 * 	mod_timer_add((void*) &led2, led2.tick_ms, (void*) &led_update, &timer1_list);
 * 	mod_timer_add((void*) &led3, led3.tick_ms, (void*) &led_update, &timer2_list);
 * 	mod_timer_add((void*) &led4, led4.tick_ms, (void*) &led_update, &timer2_list);
 *
 * 	The above code will add two led objects in each timer and the callback function
 * 	will be triggered for each one.
 *
 *  Last and most important is that you need a read hw timer to run the timer's
 * 	internal clock. Therefore, for your platform you can setup a HW timer to triggered
 * 	every 1ms and in this timer routine you just need to run these two functions.
 *
 * 	mod_timer_polling(timer1_list);
 * 	mod_timer_polling(timer2_list);
 *
 * 	It's just simple as that.
 */


#ifndef __TIMER_SCHED_H_
#define __TIMER_SCHED_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "LICENSE.h"
#include "list.h"

typedef void (*fp_timeout_cb)(void *);

/**
 * @brief: Basic timer object.
 * @parent void* This is a pointer to the object that will use the timer
 * @timeout uint16_t This is the number or the main clock ticks for the timer to be triggered
 * @counter uint16_t The count up value that is compared with the timeout_ticks
 * @fp_timeout_cb void(*)(void*) The callback function to call when the timer triggers
 * @list list_head The timer list
 */
struct obj_timer_t {
	void 				*parent;
	uint16_t 			timeout_ticks;
	volatile uint16_t 	counter;
	fp_timeout_cb		cbk;
	struct list_head 	list;
};

#define TIMER_EXISTS(TMR, ITTERATOR) ( (TMR->cbk == ITTERATOR->cbk) && (TMR->timeout_ticks == ITTERATOR->timeout_ticks) )

static inline struct obj_timer_t * __attribute__((always_inline))
mod_timer_find_timer(struct obj_timer_t * tmr, struct list_head * timer_list)
{
	if (!list_empty(timer_list)) {
		struct obj_timer_t * tmr_it = NULL;
		list_for_each_entry(tmr_it, timer_list, list) {
			if TIMER_EXISTS(tmr, tmr_it) {
				/* found */
				return(tmr_it);
			}
		}
	}
	return NULL;
}

static inline void __attribute__((always_inline))
mod_timer_add(void * object, uint16_t timeout, fp_timeout_cb obj_callback, struct list_head * timer_list)
{
	struct obj_timer_t timer = {
		.parent = object,
		.timeout_ticks = timeout,
		.counter = 0,
		.cbk = obj_callback
	};
	/* Check if already exists */
	if (!timer_list) return;
	if (!mod_timer_find_timer(&timer, timer_list)) {
		struct obj_timer_t * new_timer = (struct obj_timer_t *) malloc(sizeof(struct obj_timer_t));
		memcpy(new_timer, &timer, sizeof(struct obj_timer_t));
		// TRACE(("Timer add: %d/%d\n", new_timer->timeout_ticks, new_timer->counter));
		INIT_LIST_HEAD(&new_timer->list);
		list_add(&new_timer->list, timer_list);
	}
}

static inline void __attribute__((always_inline))
mod_timer_del(struct obj_timer_t * timer, struct list_head * timer_list)
{
	struct obj_timer_t * found_timer = mod_timer_find_timer(timer, timer_list);
	if (found_timer) {
		/* remove */
		list_del(&found_timer->list);
		free(found_timer);
	}
}

static inline void __attribute__((always_inline))
mod_timer_polling(struct list_head * timer_list)
{
	if (!list_empty(timer_list)) {
		struct obj_timer_t * tmr_it = NULL;
		list_for_each_entry(tmr_it, timer_list, list) {
			if ((++tmr_it->counter) >= tmr_it->timeout_ticks) {
				tmr_it->counter = 0;
				tmr_it->cbk(tmr_it->parent);
			}
		}
	}
}

#ifdef	__cplusplus
}
#endif

#endif /* __TIMER_SCHED_H_ */
