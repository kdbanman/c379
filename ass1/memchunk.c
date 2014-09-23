/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 1
 *
 */

/*
 * Assumptions and notes:
 *
 * - getPageSize() is constant across all execution time of process
 * - memchunk.length will always be >= getPageSize()
 * - memchunk.length will always be == 0 in (mod getPageSize())
 * - no contiguous memchunks in the return array have the same RW
 * - page size may not divide 2^32 evenly
 * - iterating to 2^32 requires a 64 bit counter, but that cannot be
 *   cast to a pointer
 */

/*
 * General approach:
 *
 * Set a pointer to the first address of each page in the address space.
 * For each of those addresses, try accessing, then reading, then writing.
 * If a segfault is intercepted, record the permission level.
 * If the permission level is different from the previos one, then a new chunk
 * has been found at the current page:
 *     - Set the final length of the current chunk
 *     - Move to the next chunk
 * Otherwise, the page is part of the current chunk.
 *     - Increase the length of the current chunk.
 */

#include <unistd.h>
#include <setjmp.h>
#include <stdio.h>
#include "memchunk.h"

#define UNKNOWN 0
#define NOACC 1
#define READ 2

/* Environment buffer for jump operations */
jmp_buf env;

/* Handler function intended for read-triggered segfaults */
void read_err_handler(int sig)
{
    longjmp(env, NOACC); /* No access to address was possible */
}

/* Handler function intended for write-triggered segfaults */
void write_err_handler(int sig)
{
    longjmp(env, READ); /* Only read access to address was possible */
}

int get_mem_layout(struct memchunk *chunk_list, int size)
{
    /* Declare and initialize constants */
    //TODO: CONVERT LONG LONG BOUNDARY AND COUNTER TO CHAR POINTER.  KEEP IN MIND POINTER ARITHMETIC AND OVERFLOW POSSIBILITY.
    const unsigned long long MAX_ADDR = 4294967295LU;
    const int PAGE_SIZE = getpagesize();
    
    unsigned long long currAddr = 0LU;

    for (; currAddr <= MAX_ADDR; currAddr += PAGE_SIZE) {
        /* Set the jump point with the current stack */
        int permission = setjmp(env);
        /* Switch on values from error handlers for long jump. */
        switch(permission)
        {
            case UNKNOWN: /* Page is not yet analyzed */
                /* Set segfault handler to read error handler */

                /* Try to read from current address */

                /* Set segfault handler to write error handler */

                /* Try to write to current address */

                /* If execution has reached here, the current page is
                 * readable and writable */
                break;
            case NOACC: /* Page is not accessible */

                break;
            case READ: /* Page has read access but no write access */
                break;
        }
    }
    
    return 0;
}
