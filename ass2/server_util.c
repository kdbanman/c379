
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#define DEBUG 1

#include <errno.h>
#include <time.h>
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server_util.h"
#include "server_conf.h"

char sentinel_400;
char sentinel_403;
char sentinel_404;
char sentinel_500;

void debug(char * action)
{
        if (DEBUG) printf("--%s--\n", action);
}

int listeningSock(serverconf conf)
{
        struct sockaddr_in add;
        int sd;
        int opt;

        memset(&add, 0, sizeof(add));
        add.sin_family = AF_INET;
        add.sin_port = htons(conf.port);
        add.sin_addr.s_addr = htonl(INADDR_ANY);

        /* Get socket descriptor. */
        sd = socket(AF_INET, SOCK_STREAM, 0);
        if (sd == -1) {
                fprintf(stderr, "ERROR: Socket creation failed.\n");
                exit(1);
        }

        /* Set socket as reusable. */
        opt = 1;
        if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
                fprintf(stderr, "ERROR: Socket option set failed.\n");
                exit(1);
        }

        /* Bind socket to desired address. */
        if (bind(sd, (struct sockaddr *) &add, sizeof(add)) == -1) {
                fprintf(stderr, "ERROR: Socket bind to address failed.\n");
                exit(1);
        }

        if (listen(sd, 127) == -1) {
                fprintf(stderr, "ERROR: Could not listen to socket.\n");
        }

        return sd;
}

char * fmttime(const struct tm *timeptr)
{
        /* static char arrays thread safe here because they are read only. */
        static const char wday_name[7][3] = {
                "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
        };
        static const char mon_name[12][3] = {
                "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };

        char * result = (char *) malloc(25 * sizeof(char));

        sprintf(result,
                "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
                wday_name[timeptr->tm_wday],
                mon_name[timeptr->tm_mon],
                timeptr->tm_mday, timeptr->tm_hour,
                timeptr->tm_min, timeptr->tm_sec,
                1900 + timeptr->tm_year);

        return result;
}

char * currtime()
{
        char * timestr;
        time_t curr;

        time(&curr);
        timestr = fmttime(gmtime(&curr));
        
        return timestr;
}

char * getAddress(struct sockaddr_in address)
{
        char * addr = (char *) malloc(INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(address.sin_addr), addr, INET_ADDRSTRLEN);
        return addr;
}

char * getRequestLine(char * req, int reqLength)
{
        int i, start, end, done;
        char * reqLine;

        i = start = end = done = 0;

        /* Locate start of actual request (ignore whitespace). */
        while (!done && i < reqLength) {
                if (req[i] != ' ' &&
                    req[i] != '\n' &&
                    req[i] != '\r' &&
                    req[i] != '\t') {
                        start = i;
                        done = 1;
                }
                i++;
        }

        /* Locate end of request line (locate CRLF start) */
        done = 0;
        while (!done && i < reqLength) {
                if (req[i] == '\n') {
                        end = i;
                        if (i > 0 && req[i-1] == '\r') {
                                end = i - 1;
                        }
                        done = 1;
                }
                i++;
        }

        /* Copy start to end if valid, else set emptystring for return. */
        if (start < end) {
                reqLine = (char *) malloc((end - start + 1) * sizeof(char));
                memcpy(reqLine, &req[start], end - start);
                reqLine[end - start] = '\0';
        } else {
                reqLine = (char *) malloc(sizeof(char));
                reqLine[0] = '\0';
        }

        return reqLine;
}

char * getResourcePath(char * req)
{
        /*
         * Regex spec notes by string line number:
         * 2. GET may be preceded by [CR]LF.
         * 3. Second subexpression \\([^[:space:]]*\\) is the resource path.
         * 4. Any number of headers are allowed.
         * 4. Line is header iff it is a colon separated pair of keyval
         *    strings followed by [CR]LF
         * 4. keyval strings contain no control characters (x01-x1f)
         * 4. This liberal definition passes the string ":\n" as a header
         *    due to the colon and the newline.
         */
        char * spec = "^"
                      "\\(\r\\{0,1\\}\n\\)*"
                      "GET \\([^[:space:]]*\\) HTTP/1\\.1\r\\{0,1\\}\n"
                      "\\([^[:cntrl:]]*:[^[:cntrl:]]*\r\\{0,1\\}\n\\)*"
                      "\\(\r\\{0,1\\}\n\\)\\{1,\\}"
                      "$";

        /* Initialize regex. */
        regex_t regex;
        int reti;
        char msgbuf[100];

        regmatch_t matches[3];

        char * reqpath;
        size_t pathsize;

        /* Compile regular expression */
        reti = regcomp(&regex, spec, 0);
        if (reti) {
                regerror(reti, &regex, msgbuf, sizeof(msgbuf));
                fprintf(stderr, "Could not compile regex: %s\n", msgbuf);
                exit(1);
        }

        /* Execute regular expression */
        reti = regexec(&regex, req, 3, matches, 0);
        if (!reti) {
            
                /* Match was found, construct the path string from the 3rd
                 * entry of the matches array. */
                pathsize = matches[2].rm_eo - matches[2].rm_so;
                reqpath = (char *) malloc((pathsize + 1) * sizeof(char));
                memcpy(reqpath, &req[matches[2].rm_so], pathsize);
                reqpath[pathsize] = '\0';
        }
        else if (reti == REG_NOMATCH) {
                /* Match was not found, so the request is invalid. */
                reqpath = ERR400;
        }
        else {
                /* Regex library experienced unexpected failure. */
                regerror(reti, &regex, msgbuf, sizeof(msgbuf));
                fprintf(stderr, "Regexlibrary unexpected failure.\n");
                fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        }

        /* Free compiled regular expression. */
        regfree(&regex);

        return reqpath;
}

request * parseGet(char * req, int length, struct sockaddr_in address)
{
        request * sreq;

        sreq = (request *) malloc(sizeof (request));

        sreq->address = getAddress(address);
        sreq->timestring = currtime();
        sreq->resourcepath = getResourcePath(req);
        sreq->requestline = getRequestLine(req, length);
        sreq->validrequest = sreq->resourcepath != NULL;

        return sreq;
}

int isFreeable(char * ptr)
{
        if (ptr == NULL || 
            ptr == ERR400 ||
            ptr == ERR403 ||
            ptr == ERR404 ||
            ptr == ERR500) return 0;

        return 1;
}

void freeRequest(request * req)
{
        if (isFreeable(req->address)) free(req->address);
        if (isFreeable(req->timestring)) free(req->timestring);
        if (isFreeable(req->requestline)) free(req->requestline);
        if (isFreeable(req->resourcepath)) free(req->resourcepath);
        if (req != NULL) free(req);
}

char * constructResponse(int code, char * message, int msgLen)
{
        char * resp, * template, * codeLine, * date;

        debug("CODE");

        /* Set code line, message, and mesage length according to code. */
        if (code == 200) {
                codeLine = "200 OK";

        } else if (code == 400) {
                codeLine = "400 Bad Request";
                message = "Malformed HTTP request.";
                msgLen = 23;

        } else if (code == 403) {
                codeLine = "403 Forbidden";
                message = "Access to requested document denied.";
                msgLen = 36;

        } else if (code == 404) {
                codeLine = "404 Not Found";
                message = "Requested document does not exist.";
                msgLen = 34;

        } else if (code == 500) {
                codeLine = "500 Internal Server Error";
                if (message == NULL) {
                        message = "Error processing request.";
                        msgLen = 25;
                }

        } else {
                codeLine = "500 Internal Server Error";
                message = "Unknown response code.";
                msgLen = 22;

                //TODO log 500
                fprintf(stderr, "Error: Unknown response code %d\n", code);
        }

        debug("DATE");

        date = currtime();

        debug("ALLOCATE");

        /* Allocate for response with 200 bytes for header and unexpectedness */
        resp = (char *) malloc((msgLen + 200) * sizeof ' ');

        template = "HTTP/1.1 %s\nDate: %s\n"
                   "Content-Type: text/html\nContent-Length: %d\n\n%s";

        debug("FILL");

        sprintf(resp, template, codeLine, date, msgLen, message);

        return resp;
}

int getContents(char * basePath, char * relPath, char ** contents)
{
        int size, sizeRead, file;
        char path[2048]; /* URLs > 2000 characters not widely supported. */

        /* Get entire path. Server directory is always slash-terminated. */
        strcpy(path, basePath);
        if (relPath[0] == '/') strcat(path, relPath + 1);
        else strcat(path, relPath);

        debug("TEST");
        
        /* Test if file exists and is not a device, socket, FIFO, etc. */
        if (!isRegFile(path)) {
                *contents = ERR404;
                return -1;
        }

        debug("OPEN");

        file = open(path, O_RDONLY);

        if (file != -1) {
                debug("SIZE");

                /* Get file size. */
                size = filesize(path);
                if (size == -1) {
                        *contents = ERR500;
                        return -1;
                }

                debug("ALLOCATE");

                /* Allocate room for file contents. */
                *contents = (char *) malloc(size);

                debug("READ");

                /* Attempt to read from file */ 
                sizeRead = read(file, *contents, size);
                if (sizeRead != size) {
                        *contents = ERR500;
                        return -1;
                }

                debug("CLOSE");
                
                close(file);
                return size;

        } else if (errno == EACCES) {
                /* Could not access file. */
                *contents = ERR403;
                return -1;
        } else if (errno == ENOENT) {
                /* Could not find file. */
                *contents = ERR404;
                return -1;
        } else {
                debug("ERROR");

                /* Something unexpected happened. */
                *contents = ERR500;
                return -1;
        }

}

int isRegFile(char * fname)
{
        struct stat st;

        if (stat(fname, &st) == 0) {
                if (st.st_mode & S_IFREG) 
                    return 1;
        }

        return 0;
}

int filesize(char * fname)
{
        struct stat st;

        if (stat(fname, &st) == 0) {
                if (st.st_mode & S_IFREG) 
                        return st.st_size;
        }

        return -1;
}

void sockWrite(request * req, int csd, char * msg, char ** log)
{
        int len;
        ssize_t w, written;

        debug("SEND");

        w = 0;
        written = 0;
        len = strlen(msg);
        while (written < len) {
                w = write(csd, msg + written, len - written);
                if (w == -1) {
                        *log = log500Msg("Socket write failed.");
                        break;
                } else {
                        written += w;
                }
        }
        *log = logMsg(200, req, written, len);
}

char * log500Msg(char * msg)
{
        //TODO use server time

        return (char *) malloc(5);
}

char * logMsg(int code, request * req, int written, int total)
{
        // TODO use time, add, request line from req

        return (char *) malloc(5);
}

void handleRequest(serverconf conf, int csd, saddr clientAdd, char ** log)
{
        //TODO returns are bad
        int fread;
        char recvbuf[4096], * msgPtr, * respPtr;
        ssize_t r;
        request * req;

        /* Set default response to catch unanticipated behaviour. */
        msgPtr = ERR500;

        debug("ACCEPT");

        if (csd == -1) {
                *log = log500Msg("Socket accept failed.");
                /* No message receivable, nothing to respond to. */
                return;
        }

        debug("READ");

        /* Read message sent by client upon connection. */
        r = read(csd, recvbuf, sizeof recvbuf);

        if (r == -1) {
                /* No message recieved, 500 response possible. */
                respPtr = constructResponse(500, "Socket read failed.", 19);
                *log = log500Msg("Socket read failed\n");
        } else if (r == 0) {
                /* No message recieved, no response possible.
                 * Close socket and return.
                 */
                close(csd);
                *log = log500Msg("Client hung up\n");
                return;
        } else {
                debug("PARSE");

                /* Get request struct from received message. */
                req = parseGet(recvbuf, r, clientAdd);

                //TODO keep looking at control flow, removing as many
                //returns as possible
                
                /* Examine if request was valid. */
                if (!req->validrequest) {
                        debug("400");

                        /* Invalid request - construct 400 response. */
                        respPtr = constructResponse(400, NULL, 0);

                        /* Construct log and free request struct. */
                        *log = logMsg(400, req, 0, 0);
                        freeRequest(req);
                        return;
                } else {
                        /* Request was valid.
                         * Attempt to read file into mem at pointer msgPtr.
                         */
                        fread = getContents(conf.basedir,
                                                   req->resourcepath,
                                                   &msgPtr);
                        
                        /* Examine file read results, constructing response. */
                        if (fread != -1) {
                                debug("200");

                                /* Success - construct 200 OK response. */
                                respPtr = constructResponse(200, msgPtr, fread);
                                //TODO log 200
                        } else if (msgPtr == ERR403) {
                                debug("403");

                                /* No permissions - construct 403. */
                                respPtr = constructResponse(403, NULL, 0);
                                //TODO log 403
                        } else if (msgPtr == ERR404) {
                                debug("404");

                                /* No file - construct 404. */
                                respPtr = constructResponse(404, NULL, 0);
                                //TODO log 404
                        } else if (msgPtr == ERR500) {
                                debug("500");

                                /* Error - construct 500. */
                                respPtr = constructResponse(500, NULL, 0);
                                *log = log500Msg("Failure reading file.");
                        }
                }
        }
        debug("SEND");

        /* Write response to client socket. */
        sockWrite(req, csd, respPtr, log);

        debug("CLEAN");

        if (isFreeable(respPtr)) free(respPtr);
        if (isFreeable(msgPtr)) free(msgPtr);
        freeRequest(req);

        close(csd);
}
