
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
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>
#include "log_monitor.h"

void safeWrite(safefile * dest, char * toWrite)
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
