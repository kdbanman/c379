/*
 * Copyright (c) 2008 Bob Beck <beck@obtuse.com>
 * Modified 2014 by Kirby Banman <kdbanman@ualberta.ca>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../ass2/server_util.h"
#include "../../ass2/server_conf.h"

/*
 * NOTE:  client is hanging up on google chrome.  not sure why.
 * WHO CARES
                // DEBUG accept() is accepting connections from google chrome when it shouldn't...
 */

int main(int argc,  char *argv[])
{
        serverconf conf;

        request * req;

	struct sockaddr_in sockname, client;
	char buffer[2000];
	socklen_t clientlen;
        int sd;

        conf = getConfig(argc, argv);

	/* the message we send the client */
	strncpy(buffer,
	    constructResponse(200, "go fuck yourself", 16),
	    2000);

	memset(&sockname, 0, sizeof(sockname));
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(conf.port);
	sockname.sin_addr.s_addr = htonl(INADDR_ANY);
	sd=socket(AF_INET,SOCK_STREAM,0);
	if ( sd == -1)
		err(1, "socket failed");
        
        int opt = 1;
        if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
                err(1, "socket option set failed");
        
	if (bind(sd, (struct sockaddr *) &sockname, sizeof(sockname)) == -1)
		err(1, "bind failed");

	if (listen(sd,127) == -1)
		err(1, "listen failed");

	/*
	 * we're now bound, and listening for connections on "sd" -
	 * each call to "accept" will return us a descriptor talking to
	 * a connected client
	 */


	/*
	 * finally - the main loop.  accept connections and deal with 'em
	 */
	printf("Server up on port %u\n", conf.port);

        while (1) {
                char * recvbuf = calloc(2000, sizeof(char));
                int clientsd;
                clientlen = sizeof(&client);
                // DEBUG accept() is accepting connections from google chrome when it shouldn't...
                clientsd = accept(sd, (struct sockaddr *)&client, &clientlen);
                if (clientsd == -1)
                        err(1, "accept failed");

                ssize_t written, w, rr;
                /*
                 * read from client
                 */
                printf("reading from client\n");
                rr = read(clientsd, recvbuf, 2000 * sizeof(char));

                if (rr == -1) {
                        err(1, "read failed");
                } else if (rr == 0) {
                        fprintf(stderr, "CLIENT HUNG UP AFTER:\n\n%s", recvbuf);
                        fprintf(stderr, "\n\n------------ENDHUP\n");
                } else {
                        printf("read complete\n");

                        printf("%.*s\n", rr, recvbuf);
                        req = parseGet(recvbuf, rr, client);

                        printf("--ADDRESS--\n");
                        printf("%s\n", req->address);
                        printf("--TIMESTRING--\n");
                        printf("%s\n", req->timestring);
                        printf("--REQUESTLINE--\n");
                        printf("%s\n", req->requestline);
                        printf("--RESOURCEPATH--\n");
                        if (req->validrequest) printf("%s\n", req->resourcepath);
                        printf("--VALID--\n");
                        printf("%d\n", req->validrequest);

                        freeRequest(req);
                }

                free(recvbuf);

                /*
                 * write the message to the client, being sure to
                 * handle a short write, or being interrupted by
                 * a signal before we could write anything.
                 */
                printf("writing to client\n");
                w = 0;
                written = 0;
                while (written < strlen(buffer)) {
                        w = write(clientsd, buffer + written,
                            strlen(buffer) - written);
                        if (w == -1)
                                err(1, "write failed");
                        else
                                written += w;
                }
                printf("write complete\n");
                
                close(clientsd);
        }
        close(sd);
}
