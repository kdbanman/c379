/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 1
 *
 */

#include <unistd.h>
#include <stdio.h>
#include "memchunk.h"

/*
 * Constants
 */
unsigned long MAX_ADDR = 4294967295LU;
int PAGE_SIZE = getpagesize();

/*
 * Assumptions and notes:
 *
 * - getPageSize() is constant across all execution time of process
 * - memchunk.length will always be >= getPageSize()
 * - memchunk.length will always be == 0 in (mod getPageSize())
 * - no contiguous memchunks in the return array have the same RW
 * - page size may not divide 2^32 evenly
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

int get_mem_layout(struct memchunk *chunk_list, int size)
{
    unsigned long currAddr = 0LU;

    for (; currAddr < MAX_ADDR; currAddr += PAGE_SIZE) {
    }
    
    return 5 / 2;
}
