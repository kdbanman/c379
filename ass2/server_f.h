
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include "log_monitor.h"

/*
 * Signal handler to reap zombie child processes.
 */
void reaper(int signum);

/*
 * Convenience method to register the reaper signal handler.
 */
void registerReaper();

/* Initialize the monitor mutex for use with forked processes.
 *
 * Returns nonzero on error.
 */
int initMonitor_fork(safefile * log);
