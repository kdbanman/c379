
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "server_conf.h"
#include "server_util.h"
#include "log_monitor.h"
#include "server_p.h"

int initMonitor_pthread(safefile * safeLog) {
        int err;
        pthread_mutexattr_t attr;

        /* Initialize mutex for use in shared memory. */
        err = pthread_mutexattr_init(&attr);
        err += pthread_mutex_init(&safeLog->busy, &attr);

        if (err) return -1;
        return 0;
}

void * handleClient(void * arg)
{
        thread_args * args = (thread_args *) arg;
        char * logMsg;

        /* Handle request and log result in child process. */
        handleRequest(args->config,
                      args->clientsd,
                      args->clientaddr,
                      &logMsg);

        safeWrite(&(args->safeLog), logMsg);
        
        if (isFreeable(logMsg)) free(logMsg);

        close(args->clientsd);

        free(args);

        return NULL;
}

int main(int argc, char *argv[])
{
        /* Master server variables. */
        serverconf config;
        int sd;
        safefile safeLog;

        /* Per request variables. */
        saddr clientaddr;
        socklen_t clientlen;
        int clientsd;

        /* Thread variables. */
        pthread_t thread;
        thread_args * args;

        config = getConfig(argc, argv);

        /* Initialize concurrency-protected log file. */
        if (initMonitor_pthread(&safeLog)) {
                fprintf(stderr, "ERROR: Could not initialize log monitor.\n");
                exit(1);
        }
        safeLog.filepath = config.logloc;

        sd = listeningSock(config);

        printf("Serving from %s on port %d, logging to %s\n",
               config.basedir,
               config.port,
               config.logloc);

        /* Daemonize. I choose not to reroute std* to /dev/null/ to 
         * help the marking TAs diagnose problems. */
        daemon(1, 1);

        clientlen = sizeof(&clientaddr);
        while (1) {
                clientsd = accept(sd,
                                  (struct sockaddr *) &clientaddr,
                                  &clientlen);
                
                /* Copy and/or reference arguments for client handler thread. */
                /* Struct memory is freed within the handler itself. */
                args = (thread_args *) malloc(sizeof(thread_args));
                args->config = config;
                args->clientsd = clientsd;
                args->clientaddr = clientaddr;
                args->safeLog = safeLog;
                
                pthread_create(&thread, NULL, handleClient, (void *) args);
                pthread_detach(thread);
        }

        close(sd);

        return 0;
}
