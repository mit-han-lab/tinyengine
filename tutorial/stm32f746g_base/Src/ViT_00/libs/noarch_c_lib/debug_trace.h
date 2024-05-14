/**
 * debug_trace.h
 *
 * LICENSE: MIT
 *
 * This header lib can be used to print trace messages using printf. It supports multiple\
 * levels that can be enabledand disabled in run-time. The default trace level is the:
 * TRACE_LEVEL_DEFAULT and this is defined in this header. You can define more trace levels
 * but prefer to do that in your code and not in the header file, so the code is re-usable.
 *
 * Finally, you also need to define the trace_levels in you code file.
 *
 * Dimitris Tassopoulos <dimtass@gmail.com>
 */

#ifndef __DEBUG_TRACE_H_
#define __DEBUG_TRACE_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "LICENSE.h"

#define DEBUG_TRACE

#define TRACE_LEVEL_DEFAULT (1 << 0)
/** You can define more in your code, e.g.
#define TRACE_LEVEL_USB 	(1 << 1)
#define TRACE_LEVEL_GPIO 	(1 << 2)
*/

#ifdef DEBUG_TRACE
#define TRACE(X) TRACEL(TRACE_LEVEL_DEFAULT, X)
#define TRACEL(TRACE_LEVEL, X) do { if (trace_levels & TRACE_LEVEL) printf X;} while(0)
#else
#define TRACE(X)
#define TRACEL(X,Y)
#endif

extern uint32_t trace_levels;

static inline void trace_levels_set(uint32_t level, uint8_t enable)
{
	if (enable) {
		trace_levels |= level;
	}
	else {
		trace_levels &= ~level;
	}
}

#ifdef __cplusplus
}
#endif

#endif //__DEBUG_TRACE_H_
