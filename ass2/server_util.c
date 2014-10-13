
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include "server_util.h"

void usageErr(char * cmd)
{
                fprintf(stderr, "Usage: %s <port> <fileroot> <logpath>\n", cmd);
}

struct serverconf getConfig(int argc, char *argv[])
{
        struct serverconf config;

        /* Ensure correct number of arguments passed. */
        if (argc != 4)
        {
                usageErr(argv[0]);
                exit(1);
        }

        /* Ensure valid port number passed. */
        config.port = atoi(argv[1]);
        if (!validPort(config.port))
        {
                usageErr(argv[0]);
                fprintf(stderr, "  port must be between 1 and 65535 (incl)\n");
                exit(1);
        }

        /* Ensure valid file server root directory passed. */
        config.basedir = argv[2];
        if (!validBaseDir(config.basedir))
        {
                usageErr(argv[0]);
                fprintf(stderr, "  fileroot must be a readable directory\n");
                fprintf(stderr, "  %s", config.basedir);
                perror("  ");
                exit(1);
        }

        /* Ensure valid log file location passed. */
        config.logloc = argv[3];
        if (!validLogFile(config.logloc))
        {
                usageErr(argv[0]);
                fprintf(stderr, "  logpath must be a writable file location\n");
                fprintf(stderr, "  %s", config.logloc);
                perror("  ");
                exit(1);
        }

        return config;
}

int validPort(int port)
{
        /* Return 0 if port is out of range, 1 otherwise. */
        if (port >= 1 && port <= 65535) return 1;
        else return 0;
}

int validBaseDir(char * path)
{
        DIR * dir;

        /* Attempt to open directory, returning 1 on success. */
        dir = opendir(path);
        if (dir == NULL)
        {
                return 0;
        } else {
                closedir(dir);
                return 1;
        }
}

int validLogFile(char * path)
{
        FILE * fp;

        /* Attempt to open log file, returning 1 on success. */
        fp = fopen(path, "a");
        if (fp == NULL) {
                return 0;
        } else {
                fclose(fp);
                return 1;
        }
}

void safeWrite(struct safefile * dest, char * toWrite)
{
        FILE * fp;
            
        /* Lock the log file's mutex and write the passed string. */
        pthread_mutex_lock(&(dest->busy));

        fp = fopen(dest->filepath, "a");
        if (fp != NULL)
        {
                fputs(toWrite, fp);
                fclose(fp);
        } else {
                fprintf(stderr, "Could not write to %s\n", dest->filepath);
        }

        /* Writing is done - unlock the mutex for the next call. */
        pthread_mutex_unlock(&(dest->busy));
}
