
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

struct serverconf {
        int port;
        char * basedir;
        char * logloc;
};

/*
 * Parse and validate command line arguments to obtain runtime configuration.
 * Invalid arguments terminate program with error code 1.
 */
struct serverconf getConfig(int argc, char *argv[]);

/*
 * Returns nonzero (true) if the passed port is between 0 and 65535 inclusive.
 */
int validPort(int port);

/*
 * Returns nonzero (true) if the passed string is a valid directory or file.
 */
int validPath(char * path);

/*
 * Returns nonzero (true) if the passed string is a readable directory.
 */
int validBaseDir(char * path);

/*
 * Returns nonzero (true) if the passed string is a writable file.
 */
int validLogFile(char * path);

