
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include <pthread.h>

struct serverconf {
        int port;
        char * basedir;
        char * logloc;
};

struct safefile {
        char * filepath;
        pthread_mutex_t busy;
};

/*
 * Print the correct usage of the program.
 */
void usageErr(char * cmd);

/*
 * Parse and validate command line arguments to obtain runtime configuration.
 * Invalid arguments terminate program with error code 1.
 */
struct serverconf getConfig(int argc, char *argv[]);

/*
 * Returns nonzero (true) if the passed port is between 1 and 65535 inclusive.
 */
int validPort(int port);

/*
 * Returns nonzero (true) if the passed string is a readable directory.
 */
int validBaseDir(char * path);

/*
 * Returns nonzero (true) if the passed string is a writable file.
 */
int validLogFile(char * path);

/*
 * Writes a string to a file protected against concurrent writes with a mutex.
 */
void safeWrite(struct safefile * dest, char * toWrite);
