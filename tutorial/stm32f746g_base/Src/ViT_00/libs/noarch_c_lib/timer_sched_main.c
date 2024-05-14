/**
 * This is an example test project for the timer_sched.h (timer/scheduler library)
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
#include "timer_sched.h"

/* Set trace levels */
uint32_t trace_levels = \
		TRACE_LEVEL_DEFAULT |
		0;

static LIST_HEAD(obj_timer_list);

struct a_random_obj {
	char name[20];
};

void random_obj_cb(struct a_random_obj * obj)
{
	TRACE(("random_obj_cb: %s\n", obj->name));
}

int main()
{
	/* create an object */
	struct a_random_obj obj1 = {.name = "RANDOM OBJ1"};
	/* add the object to the scheduler */
	mod_timer_add((void*) &obj1, 1000, (void*) &random_obj_cb, &obj_timer_list);


	/* create another object */
	struct a_random_obj obj2 = {.name = "RANDOM OBJ2"};
	/* add the object to the scheduler */
	mod_timer_add((void*) &obj2, 500, (void*) &random_obj_cb, &obj_timer_list);


	/* run the loop. In the ouput you should see the OBJ2 2 times/sec and OBJ1 1 time/sec */
	while(1) {
		usleep(1 * 1000); //every 1msec
		mod_timer_polling(&obj_timer_list);
	}

	return 0;
}
