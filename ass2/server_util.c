
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include <time.h>
#include <regex.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "server_util.h"

char * currtime()
{
        time_t curr;
        time(&curr);
        return asctime(gmtime(&curr));
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
                reqpath = NULL;
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

request parseGet(char * req, struct sockaddr_in address)
{
 /*
  * Since HTTP GET is assumed, request line is constructed from resource
  * path.
  */
}
