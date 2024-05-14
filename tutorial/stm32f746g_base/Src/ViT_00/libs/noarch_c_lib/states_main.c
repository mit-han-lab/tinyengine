/**
 * This is an example test project for the state.h (state machine library)
 *
 * @author: Dimitris Tassopoulos <dimtass@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "LICENSE.h"
#include "various_defs.h"
#include "states.h"
#include "debug_trace.h"

/* Set trace levels */
uint32_t trace_levels = \
		TRACE_LEVEL_DEFAULT |
		0;

/* STATE_1 data and functions */
struct tp_state_1_data {
	char name[20];
};

void state_1_enter_cb(state_t prev, void * data)
{
	struct tp_state_1_data * state_data = (struct tp_state_1_data*) data;
	TRACE(("state_1_enter_cb: %s\n", state_data->name));
}

void state_1_run_cb(void * data)
{
	struct tp_state_1_data * state_data = (struct tp_state_1_data*) data;
	TRACE(("state_1_run_cb: %s\n", state_data->name));
}

void state_1_exit_cb(state_t next, void * data)
{
	struct tp_state_1_data * state_data = (struct tp_state_1_data*) data;
	TRACE(("state_1_exit_cb: %s\n", state_data->name));
}


/* STATE_2 data and functions */
struct tp_state_2_data {
	char name[20];
};

void state_2_enter_cb(state_t prev, void * data)
{
	struct tp_state_2_data * state_data = (struct tp_state_2_data*) data;
	TRACE(("state_2_enter_cb: %s\n", state_data->name));
}

void state_2_run_cb(void * data)
{
	struct tp_state_2_data * state_data = (struct tp_state_2_data*) data;
	TRACE(("state_2_run_cb: %s\n", state_data->name));
}

void state_2_exit_cb(state_t next, void * data)
{
	struct tp_state_2_data * state_data = (struct tp_state_2_data*) data;
	TRACE(("state_2_exit_cb: %s\n", state_data->name));
}


int main(void)
{
	struct tp_state_1_data data_1 = {
		.name = "DATA_1",
	};
	/* enumerator with states */
	enum en_state_1 {
		eNONE_1 = 0,
		eSTATE_1_1,
		eSTATE_1_2,
	    eEND_STATE_1,
	};
	/* a list with states */
	struct tp_state sys_states_list[] = {
	    [eNONE_1] = DECLARE_STATE(eNONE_1, &state_1_enter_cb, &state_1_run_cb, &state_1_exit_cb),
	    [eSTATE_1_1] = DECLARE_STATE(eSTATE_1_1, &state_1_enter_cb, &state_1_run_cb, &state_1_exit_cb),
	    [eSTATE_1_2] = DECLARE_STATE(eSTATE_1_2, &state_1_enter_cb, &state_1_run_cb, &state_1_exit_cb),
	};
	/* declare the state object */
	DECLARE_STATE_OBJ(sys_states, (void*) &data_1, sys_states_list, ARRAY_SIZE(sys_states_list), &sys_states_list[eNONE_1], &sys_states_list[eSTATE_1_1]);


	struct tp_state_2_data data_2 = {
		.name = "DATA_2",
	};
	/* enumerator with states */
	enum en_state_module {
		eNONE_2 = 0,
		eSTATE_2_1,
		eSTATE_2_2,

		eEND_STATE_2,
	};
	/* a list with states */
	struct tp_state module_states_list[] = {
	    [eNONE_2] = DECLARE_STATE(eNONE_2, &state_2_enter_cb, &state_2_run_cb, &state_2_exit_cb),
	    [eSTATE_2_1] = DECLARE_STATE(eSTATE_2_1, &state_2_enter_cb, &state_2_run_cb, &state_2_exit_cb),
	    [eSTATE_2_2] = DECLARE_STATE(eSTATE_2_2, &state_2_enter_cb, &state_2_run_cb, &state_2_exit_cb),
	};
	/* declare the state object */
	DECLARE_STATE_OBJ(module_states, (void*) &data_2, module_states_list, ARRAY_SIZE(module_states_list), &module_states_list[eNONE_2], &module_states_list[eSTATE_2_1]);


	while(1) {
		usleep(500 * 1000);
		state_handler(&sys_states);
		state_handler(&module_states);
	}

	return EXIT_SUCCESS;
}
