
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include <time.h>
#include <string.h>

char * currtime()
{
        time_t curr;
        time(&curr);
        return asctime(gmtime(&curr));
}

char * 

request parseGet(char * req)
{
        /* Get request line boundaries */
        /* Ensure request line is "GET <path> HTTP/1.1" */
        /* wut */
}
