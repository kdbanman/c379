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

/*
 * NOTE:  client is hanging up.  not sure why.
 */

static void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s portnumber\n", __progname);
	exit(1);
}

int main(int argc,  char *argv[])
{
        request * req;

	struct sockaddr_in sockname, client;
	char buffer[2000], recvbuf[2000], *ep;
	struct sigaction sa;
	socklen_t clientlen;
        int sd;
	u_short port;
	pid_t pid;
	u_long p;

	/*
	 * first, figure out what port we will listen on - it should
	 * be our first parameter.
	 */

	if (argc != 2)
		usage();
		errno = 0;
        p = strtoul(argv[1], &ep, 10);
        if (*argv[1] == '\0' || *ep != '\0') {
		/* parameter wasn't a number, or was empty */
		fprintf(stderr, "%s - not a number\n", argv[1]);
		usage();
	}
        if ((errno == ERANGE && p == ULONG_MAX) || (p > USHRT_MAX)) {
		/* It's a number, but it either can't fit in an unsigned
		 * long, or is too big for an unsigned short
		 */
		fprintf(stderr, "%s - value out of range\n", argv[1]);
		usage();
	}
	/* now safe to do this */
	port = p;

	/* the message we send the client */
	strncpy(buffer,
	    constructResponse(404, "go fuck yourself", 16),
	    2000);

	memset(&sockname, 0, sizeof(sockname));
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(port);
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
	printf("Server up and listening for connections on port %u\n", port);

        while (1) {
                int clientsd;
                clientlen = sizeof(&client);
                clientsd = accept(sd, (struct sockaddr *)&client, &clientlen);
                if (clientsd == -1)
                        err(1, "accept failed");

                ssize_t written, w, read;
                /*
                 * read from client
                 */
                printf("reading from client\n");
                read = recv(clientsd, recvbuf, 1000, 0);

                if (read == -1)
                        err(1, "read failed");
                if (read == 0)
                        fprintf(stderr, "client hung up after:%s", recvbuf);
                else {
                        printf("read complete\n");

                        printf("%.*s\n", read, recvbuf);
                        req = parseGet(recvbuf, read, client);

                        printf("%s\n", req->address);
                        printf("%s\n", req->timestring);
                        printf("%s\n", req->requestline);
                        if (req->validrequest) printf("%s\n", req->resourcepath);
                        printf("valid: %d\n", req->validrequest);

                        freeRequest(req);
                }


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
