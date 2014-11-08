
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include "server_util.h"

/*
 * Struct for passing arguments into client handler thread.
 */
typedef struct thread_args {
	serverconf config;
	int clientsd;
	saddr clientaddr;
	safefile safeLog;
} thread_args;

/* 
 * Initialize the monitor mutex for use with posix threads.
 *
 * Returns nonzero on error.
 */
int initMonitor_pthread(safefile * log);

/* 
 * Client handler for each thread.
 */
void * handleClient(void * arg);
