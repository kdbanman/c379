
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#include <string.h>
#include <pthread.h>
#include <sys/mman.h>

#include <stdio.h>
#include <unistd.h>
#include "server_conf.h"
#include "server_util.h"
#include "log_monitor.h"
#include "server_f.h"

void reaper(int signum)
{
        /* Suppress warning. */
        signum = signum;
        /* Reap all children in the queue. */
        while(waitpid((pid_t)(-1), 0, WNOHANG) > 0) {};
}

void registerReaper()
{
        struct sigaction sa;

        sa.sa_handler = reaper;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;

        if (sigaction(SIGCHLD, &sa, NULL) == -1)
            fprintf(stderr, "ERROR: Child handler registration failed.\n");
}

int initMonitor_fork(safefile * safeLog)
{
        int prot, flags, err;
        pthread_mutexattr_t attr;

        /* Place monitor in shared memory. */
        prot = PROT_READ | PROT_WRITE;
        flags = MAP_SHARED | MAP_ANONYMOUS;
        safeLog = mmap(NULL, sizeof(safefile), prot, flags, -1, 0);

        if (safeLog == MAP_FAILED) return -1;

        /* Initialize mutex for use in shared memory. */
        err = pthread_mutexattr_init(&attr);
        err += pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        err += pthread_mutex_init(&safeLog->busy, &attr);

        if (err) return -1;
        return 0;
}

int main(int argc, char *argv[])
{
        /* Master server variables. */
        serverconf config;
        safefile safeLog;
        int sd;
        pid_t pid;

        /* Per request variables. */
        char * logMsg;
        saddr clientaddr;
        socklen_t clientlen;
        int clientsd;

        /* Parse server configuration from cmdline args. */
        config = getConfig(argc, argv);

        /* Initialize concurrency-protected log file. */
        if (initMonitor_fork(&safeLog)) {
                fprintf(stderr, "ERROR: Could not initialize log monitor.\n");
                exit(1);
        }
        safeLog.filepath = config.logloc;

        /* Register signal handler for zombie children. */
        registerReaper();

        /* Acquire configured listening server socket. */
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
                pid = fork();
                if (pid == -1) {
                        safeWrite(&safeLog, "ERROR: fork() failed!");
                } else if (pid == 0) {
                        /* Handle request and log result in child process. */
                        handleRequest(config, clientsd, clientaddr, &logMsg);
                        safeWrite(&safeLog, logMsg);
                        
                        if (isFreeable(logMsg)) free(logMsg);

                        exit(0);
                }
                close(clientsd);
        }

        close(sd);

        return 0;
}
