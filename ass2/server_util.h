
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include <netinet/in.h>
#include <time.h>
#include "server_conf.h"

#define ERR400 (&sentinel_400)
#define ERR403 (&sentinel_403)
#define ERR404 (&sentinel_404)
#define ERR500 (&sentinel_500)

extern char sentinel_400;
extern char sentinel_403;
extern char sentinel_404;
extern char sentinel_500;

/*
 * Client request struct, to be returned by initial GET parse.
 * - address set according to socket api.
 * - timestring set according to server local time.
 * - requestline set as received.  EX. GET /ex/path.txt HTTP/1.1
 * - resourcepath set from request line (after "GET")
 * - validrequest set nonzero iff entire request is valid HTTP/1.1
 *
 * Any of the char * may be one of the error sentinels, ERRxxx.
 */
typedef struct request {
        char * address;
        char * timestring;
        char * requestline;
        char * resourcepath;
        int validrequest;
} request;

/*
 * MEMORY: Return string is malloced!
 *
 * From a time struct, returns a string of the format:
 *          Www Mmm dd hh:mm:ss yyy
 * 
 * May return ERR500.
 */
char * fmttime(const struct tm *timeptr);

/*
 * MEMORY: Return string is malloced!
 *
 * Returns a formatted string of the current time
 *
 * May return ERR500.
 */
char * currtime();

/*
 * MEMORY: Return string is malloced!
 * 
 * Returns a string of the ipv4 address found in the sockaddr parameter.
 *
 * May return ERR500.
 */

char * getAddress(struct sockaddr_in address);

/*
 * MEMORY Return string is malloced!
 *
 * Returns the first line (assumed request line) of the request string.
 *
 * May return ERR500.
 */

char * getRequestLine(char * req, int reqLength);

/*
 * MEMORY: Return string is malloced!
 *
 * Tests adherence of request string to HTTP GET spec.
 * If request is valid HTTP GET, return requested resource (path) string.
 * If request is not valid, return ERR500 sentinel.
 *
 * May return ERR400 or ERR500.
 */
char * getResourcePath(char * req);

/*
 * MEMORY: Return struct is malloced! Call freeRequest to fully destruct.
 *
 * Validates string against HTTP 1.1 GET and populates a request struct.
 *
 * May return ERR500.
 */
request * parseGet(char * req, int length, struct sockaddr_in address);

/*
 * Returns zero iff the string is a null pointer or any of the sentinels.
 */
int isFreeable(char * ptr);

/*
 * Frees all member string memory, then the struct itself.
 */
void freeRequest(request * req);

/*
 * MEMORY: Return string is malloced!
 *
 * Returns a string that is ready to send to the client.
 *
 * @param code must be one of 200, 400, 403, 404, 500.
 * @param message and msgLen only used for codes 200 and 500.
 */
char * constructResponse(int code, char * message, int msgLen);

/*
 * MEMORY: Created string is malloced!
 *
 * Reads a (null-terminated) string of the file contents, into malloced buffer,
 * then points the final parameter (contents) at that buffer.  The pointer
 * may also be pointed at one of the sentinels and return -1 on error:
 *  - ERR403
 *  - ERR404
 *  - ERR500
 *
 * Returns number of bytes read.
 *
 * @params basePath and relPath are concatenated to a URI location
 * @param contents will be modified to point at a new char bufer
 */
int getContents(char * basePath, char * relPath, char ** contents);

/*
 * Attempts to get the specified file's size.
 * 
 * Returns -1 on error.
 */
int filesize(char * fname);

/*
 * Master function to take an accepted socket descriptor, respond to it, and
 * log the result.
 */
void handleRequest(serverconf conf, int clientsd, struct sockaddr_in clientAdd);
