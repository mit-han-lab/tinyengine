/**
 * This is an example test project for the mod_led.h (timer/scheduler library)
 *
 * @author: Dimitris Tassopoulos <dimtass@gmail.com>
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "LICENSE.h"
#include "various_defs.h"
#include "debug_trace.h"
#include "mod_led.h"

/* Set trace levels */
uint32_t trace_levels = \
		TRACE_LEVEL_DEFAULT |
		0;

struct bsp_led {
	char 	port[20];
	uint8_t pin;
};

int port_init(void* bsp_data)
{
	struct bsp_led * bsp = bsp_data;
	TRACE(("port_init: %s.%d\n", bsp->port, bsp->pin));
	return bsp->pin;
}

int port_pin_low(void* bsp_data)
{
	struct bsp_led * bsp = bsp_data;
	TRACE(("port_pin_low: %s.%d\n", bsp->port, bsp->pin));
	return bsp->pin;
}

int port_pin_high(void* bsp_data)
{
	struct bsp_led * bsp = bsp_data;
	TRACE(("port_pin_high: %s.%d\n", bsp->port, bsp->pin));
	return bsp->pin;
}


int main()
{
	DECLARE_MODULE_LED(led_module,8,1000);
	mod_led_init(&led_module);

	struct bsp_led led_data = {.port = "PORTA", .pin = 12};

	DECLARE_DEV_LED(test_led, &led_module, 1, &led_data, &port_init, &port_pin_low, &port_pin_high);
	led_pattern_t blink = 0b11001010;
	dev_led_add(&test_led);
	dev_led_set_pattern(&test_led, blink);


	/* run the loop. In the ouput you should see the OBJ2 2 times/sec and OBJ1 1 time/sec */
	while(1) {
		usleep(1000 * 1000); //every 1msec
		mod_led_update(&led_module);
	}

	return 0;
}


