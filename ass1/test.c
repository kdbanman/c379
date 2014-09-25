#include <stdio.h>
#include "memchunk.h"

unsigned long NUM_CHUNKS = 5;

int main()
{
    struct memchunk chunks[NUM_CHUNKS];

    printf("thing %d\n", get_mem_layout(chunks, NUM_CHUNKS));
    
    //TODO print chunk table

    return 0;
}
