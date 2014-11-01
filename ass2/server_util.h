
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

/*
 * Client request struct, to be returned by initial GET parse.
 * - address set according to socket api.
 * - timestring set according to server local time.
 * - requestline set as received.  EX. GET /ex/path.txt HTTP/1.1
 * - resourcepath set from request line (after "GET")
 * - validrequest set nonzero iff entire request is valid HTTP/1.1
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
 */
char * fmttime(const struct tm *timeptr);

/*
 * MEMORY: Return string is malloced!
 *
 * Returns a formatted string of the current time
 */
char * currtime();

/*
 * MEMORY: Return string is malloced!
 * 
 * Returns a string of the ipv4 address found in the sockaddr parameter.
 */

char * getAddress(struct sockaddr_in address);

/*
 * MEMORY Return string is malloced!
 *
 * Returns the first line (assumed request line) of the request string.
 */

char * getRequestLine(char * req, int reqLength);

/*
 * MEMORY: Return string is malloced!
 *
 * Tests adherence of request string to HTTP GET spec.
 * If request is valid HTTP GET, return requested resource (path) string.
 * If request is not valid, return NULL.
 */
char * getResourcePath(char * req);

/*
 * MEMORY: Return struct is malloced! Call freeRequest to fully destruct.
 *
 * Validates string against HTTP 1.1 GET and populates a request struct.
 */
request * parseGet(char * req, int length, struct sockaddr_in address);

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
 * @param message, msgLen only used for code 200
 */
char * constructResponse(int code, char * message, int msgLen);
