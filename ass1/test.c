#include <stdio.h>
#include "memchunk.h"

#define NUM_CHUNKS 22

int main()
{
        struct memchunk chunks[NUM_CHUNKS];

        printf("%d chunks total.\n", get_mem_layout(chunks, NUM_CHUNKS));
        printf("%d chunks recorded:\n", NUM_CHUNKS);

        printf("----------------------------------------------------\n");
        printf("| Chunk |          Start |         Length |     RW |\n");
        printf("----------------------------------------------------\n");

        int i;
        for (i = 0; i < NUM_CHUNKS; i++)
        {
                void * start = chunks[i].start;
                unsigned long length = chunks[i].length;
                int RW = chunks[i].RW;

                printf("| %5d | %14p | %14lu | %6d |\n", i, start, length, RW);
        }

        printf("----------------------------------------------------\n");
        
        return 0;
}
