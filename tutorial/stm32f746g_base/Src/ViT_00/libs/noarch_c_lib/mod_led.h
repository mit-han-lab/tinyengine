/*
 * mod_led.h
 * 
 * Usage:
 * // Declare LED module and initialize it
 * DECLARE_MODULE_LED(led_module, 8, 250);
 * mod_led_init(&led_module);
 * mod_timer_add((void*) &led_module, led_module.tick_ms, (void*) &mod_led_update, &obj_timer_list);
 * // Declare LED
 * DECLARE_DEV_LED(def_led, &led_module, 1, NULL, &led_init, &led_on, &led_off);
 * dev_led_add(&def_led);
 * dev_led_set_pattern(&def_led, 0b11001100);
 */

#ifndef __MOD_LED_H_
#define __MOD_LED_H_

#include <stdint.h>
#include "LICENSE.h"
#include "list.h"

/* This defines the size/bit-length of the pattern */
typedef uint16_t led_pattern_t;

#define DECLARE_MODULE_LED(NAME,SIZE,TICK_MS) \
	struct mod_led NAME = { \
		.pattern_size = SIZE, \
		.tick_ms = TICK_MS, \
		.pattern_index = 0, \
	}


struct mod_led {
	volatile uint8_t pattern_index;
	uint8_t		pattern_size;
	uint16_t	tick_ms;
	struct list_head led_list;
};


#define DECLARE_DEV_LED(NAME,OWNER,ID,BSP_DATA,INIT_FUNC,ON_FUNC,OFF_FUNC) \
	struct dev_led NAME = { \
		.owner = OWNER, \
		.id = ID, \
		.bsp_data =BSP_DATA, \
		.led_init = INIT_FUNC, \
		.led_on = ON_FUNC, \
		.led_off = OFF_FUNC, \
		.pattern = 0, \
	}


struct dev_led {
	struct mod_led * 	owner;
	uint16_t			id;
	led_pattern_t		pattern;
	void 				*bsp_data;
	void (*led_init)(void*);
	void (*led_on)(void*);
	void (*led_off)(void*);
	struct list_head 	list;
};


#define LED_EXISTS(LED, ITTERATOR) (LED->id == ITTERATOR->id)

/**
 * @brief Initialize LED
 * @param[in] dev_led A pointer to the LED dev
 */
static inline void  __attribute__((always_inline))
mod_led_init(struct mod_led * mod)
{
	INIT_LIST_HEAD(&mod->led_list);
}


static inline struct __attribute__((always_inline))
dev_led * dev_led_find(struct mod_led * mod, struct dev_led * led)
{
	if (!mod || !led) return NULL;
	if (!list_empty(&mod->led_list)) {
		struct dev_led * led_it = NULL;
		list_for_each_entry(led_it, &mod->led_list, list) {
			if LED_EXISTS(led, led_it) {
				/* found */
				return(led_it);
			}
		}
	}
	return NULL;
}


/**
 * @brief Set the active pattern
 * @param[in] dev_led A pointer to the LED dev struct
 * @param[in] pattern The pattern to set as active.This is truncated to uint8_t internally
 */
static inline void  __attribute__((always_inline))
dev_led_set_pattern(struct dev_led * led, led_pattern_t pattern)
{
	struct mod_led * mod = led->owner;
	struct dev_led * found_led = dev_led_find(mod, led);
	if (found_led) {
		found_led->pattern = pattern;
	}
}


static inline void  __attribute__((always_inline))
dev_led_add(struct dev_led * led)
{
	struct mod_led * mod = led->owner;

	/* do not allow duplicates */
	if (dev_led_find(mod, led)) {
		return;
	}

	/* init dev head list */
	INIT_LIST_HEAD(&led->list);

	/* Add to led_list */
	list_add(&led->list, &mod->led_list);

	/* initialize the port/pin. This is a BSP depended call and must be implemented elsewhere */
	led->led_init(led->bsp_data);

	dev_led_set_pattern(led, led->pattern);
	return;
}


static inline void  __attribute__((always_inline))
dev_led_remove(struct dev_led * led)
{
	struct mod_led * dev = led->owner;
	struct dev_led * found_led = dev_led_find(dev, led);
	if (found_led) {
		led->led_off(led->bsp_data);
		list_del(&found_led->list);
	}
}


/**
 * @brief Update the LED pattern
 */
static inline void  __attribute__((always_inline))
mod_led_update(struct mod_led * mod)
{
	if (!list_empty(&mod->led_list)) {
		struct dev_led * led_it;
		list_for_each_entry(led_it, &mod->led_list, list) {
			if (led_it->pattern & (1 << mod->pattern_index) )
				led_it->led_on(led_it->bsp_data);
			else
				led_it->led_off(led_it->bsp_data);
			if ((++mod->pattern_index) == mod->pattern_size)
				mod->pattern_index = 0;
		}
	}
}

#endif /* __MOD_LED_H_ */
