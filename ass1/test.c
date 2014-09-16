#include <stdio.h>
#include "memchunk.h"

int main()
{
    struct memchunk *chunkPtr;
    struct memchunk firstChunk;
    int numChunks;

    firstChunk.start = 0;
    firstChunk.length = 1;
    firstChunk.RW = 0;

    chunkPtr = &firstChunk;

    numChunks = 1;

    printf("thing %d\n", get_mem_layout(chunkPtr, numChunks));

    return 0;
}
