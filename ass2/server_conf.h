
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#ifndef SERVER_CONF_H_
#define SERVER_CONF_H_

typedef struct serverconf {
        int port;
        char * basedir;
        char * logloc;
} serverconf;

/*
 * Print the correct usage of the program.
 */
void usageErr(char * cmd);

/*
 * Parse and validate command line arguments to obtain runtime configuration.
 * Invalid arguments terminate program with error code 1.
 */
serverconf getConfig(int argc, char *argv[]);

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

#endif
