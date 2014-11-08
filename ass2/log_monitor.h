
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#ifndef LOG_MONITOR_H_
#define LOG_MONITOR_H_

#include <pthread.h>

typedef struct safefile {
	char * filepath;
	pthread_mutex_t busy;
} safefile;

/*
 * Writes a string to a file protected against concurrent writes with a mutex.
 */
void safeWrite(safefile * dest, char * toWrite);

#endif
