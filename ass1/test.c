/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 1
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "memchunk.h"

void printUsage(char * name)
{
	printf("Usage:\n");
	printf("  %s <integer, at least 1>\n", name);
}

int main(int argc, char **argv)
{
	
	int converted = -1;
	if (argc != 2)
	{
		printUsage(argv[0]);
		return 1;
	}
	else
	{
		converted = atoi(argv[1]);
		if (converted <= 0)
		{
			printUsage(argv[0]);
			return 1;
		}
	}

	const int NUM_CHUNKS = converted;

	struct memchunk chunks[NUM_CHUNKS];

	printf("%d array positions to fill.\n", NUM_CHUNKS);
	printf("%d chunks found.\n", get_mem_layout(chunks, NUM_CHUNKS));
	printf("Array content:\n");

	printf("----------------------------------------------------\n");
	printf("| Chunk |          Start |         Length |     RW |\n");
	printf("----------------------------------------------------\n");

	int i;
        unsigned long long totalLength = 0;
	for (i = 0; i < NUM_CHUNKS; i++)
	{
		void * start = chunks[i].start;
		unsigned long length = chunks[i].length;
		int RW = chunks[i].RW;

		printf("| %5d | %14p | %14lu | %6d |\n", i, start, length, RW);

                totalLength += length;
	}

	printf("----------------------------------------------------\n");

	printf("Total length recorded in array: %llu bytes\n", totalLength);
	
	return 0;
}
