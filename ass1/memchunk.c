/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 1
 */

#include <unistd.h>
#include "memchunk.h"

unsigned long MAX_ADDR = 4294967296;

/*
 * Assumptions and notes:
 *
 * - getPageSize() is constant across all execution time of process
 * - memchunk.length will always be >= getPageSize()
 * - memchunk.length will always be == 0 in (mod getPageSize())
 * - no contiguous memchunks in the return array have the same RW
 * 
 * - page size may not divide 2^32 evenly
 *
 */

int get_mem_layout(struct memchunk *chunk_list, int size)
{
    unsigned long finalChunk;
    unsigned long i;
    
    finalChunk = MAX_ADDR / getpagesize(); 

    // if page size does not divide 2^32 evenly, then we need to check the
    // remaining addresses.
    if (MAX_ADDR % getpagesize() != 0) finalChunk++;

    for (i = 0; i < finalChunk; i++) {

    }
    
    return 5 / 2;
}
