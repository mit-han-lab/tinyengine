/**
 * states.h
 *
 * LICENSE: MIT
 *
 * This header lib can be used to create state machines. You can use
 * this lib for multiple state machines at the same time. Basic usage:
 *
 * 1. Create an enum the states
 * 2. Create a struct tp_state[] list with the above states
 * 3. Use DECLARE_STATE_OBJ to declare a new state machine.
 *
 * Dimitris Tassopoulos <dimtass@gmail.com>
 */

#ifndef __STATES_H_
#define __STATES_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "LICENSE.h"

typedef uint16_t state_t;

struct tp_state {
	state_t state;
    void (*enter)(state_t prev, void * data);
    void (*run)(void * data);
    void (*exit)(state_t next, void * data);
};

#define DECLARE_STATE(STATE,ENTER,RUN,EXIT) { \
            .state = (state_t) STATE, \
            .enter = ENTER, \
            .run = RUN, \
            .exit = EXIT, \
        }

struct obj_state_t {
	void 				*data;
	struct tp_state 	*state_list;
	uint16_t			state_list_size;
	struct tp_state 	*state_curr;
	struct tp_state 	*state_next;
};

#define DECLARE_STATE_OBJ(NAME,DATA,LIST,LIST_SIZE,CURR,NEXT) \
	struct obj_state_t NAME = { \
		.data = DATA, \
		.state_list = LIST, \
		.state_list_size = LIST_SIZE, \
		.state_curr = CURR, \
		.state_next = NEXT, \
	}


inline void __attribute__((always_inline))
state_change(struct obj_state_t * obj, state_t  next)
{
    if (next < obj->state_list_size)
    	obj->state_next = &obj->state_list[next];
}

inline void __attribute__((always_inline))
state_handler(struct obj_state_t * obj)
{
    if (obj->state_curr != obj->state_next) {
        if (obj->state_curr->exit)
        	obj->state_curr->exit(obj->state_next->state, obj->data);
        if (obj->state_next->enter)
        	obj->state_next->enter(obj->state_curr->state, obj->data);
        obj->state_curr = obj->state_next;

        #ifdef TRACE
        TRACE(("state: %d -> %d\n", obj->state_curr->state, obj->state_next->state));
        #endif
    }
    if (obj->state_curr->run)
    	obj->state_curr->run(obj->data);
}

#ifdef	__cplusplus
}
#endif

#endif // __STATES_H_
