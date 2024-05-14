/*
 * various_defs.h
 *
 * This header file contains various useful defines., You can either include the
 * file to your project of just copy-paste parts that you need. Most of these stuff
 * are not mine, so the license might be GPL and not MIT
 */

#ifndef __VARIOUS_DEFS_H_
#define __VARIOUS_DEFS_H_

/**
 * @brief: Get the size of any array
 */
#define ARRAY_SIZE(X) (sizeof(X)/sizeof(X[0]))

#define container_of(ptr, type, member) ({ \
	const typeof(((type *)0)->member) * __mptr = (ptr); \
	(type *)((char *)__mptr - offsetof(type, member)); \
	})

#endif /* __VARIOUS_DEFS_H_ */
