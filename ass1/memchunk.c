/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 1
 */

#include <unistd.h>
#include "memchunk.h"

int get_mem_layout(struct memchunk *chunk_list, int size)
{
    return getpagesize();
}
