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
 * - getPageSize() is constant across entire execution of process
 * - memchunk.length will always be >= getPageSize()
 * - memchunk.length will always be == 0 in (mod getPageSize())
 * - no contiguous memchunks in the return array can have the same RW
 * - there may be more addresses than 2^32, but they need not be assessed
 * - page size need not divide 2^32 with zero remainder
 *   cast to a pointer
 */

/*
 * General approach:
 *
 * Set a pointer to the first address of each page in the address space.
 * For each of those addresses, try accessing, then reading, then
 * writing.
 * Record what is being attempted in a global variable.
 * If a segfault is intercepted, record the permission level.
 * If the permission level is different from the previos one, then a new
 * chunk has been found at the current page:
 *     - Set the final length of the current chunk
 *     - Move to the next chunk
 * Otherwise, the page is part of the current chunk.
 *     - Increase the length of the current chunk.
 */

#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "memchunk.h"

#define DEBUG 0

#define UNKNOWN 0
#define NOACC 1
#define READ 2

/* Environment buffer for jump operations */
jmp_buf env;

/* Tracker for attempted operations on memory */
operation attempt;

/* Provide debug level dependent print statements */
void debug(int debugLevel, const char * format, ...)
{
	if (DEBUG >= debugLevel)
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

/* Handler function intended for read-triggered segfaults */
void rw_err_handler(int sig)
{
	/* Silence unused parameter warning. */
	(void) sig;

	if (attempt == op_read)
	{
		/* Read was attempted, so no access to this address is 
                 * possible. */
		siglongjmp(env, NOACC);
	}
	else if (attempt == op_write)
	{
		/* Write was attempted after read, so only reading from
                 * this address is possible. */
		siglongjmp(env, READ);
	}
	else
	{
		/* Exit silently, because printf calls in signal handlers is
		 * unspecified behaviour */
		exit(1);
	}
}

int getPermission(unsigned long address)
{
	/* Set segfault error handler */
	struct sigaction act;

	act.sa_handler = rw_err_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGSEGV, &act, 0);

	int permission = sigsetjmp(env, 1);

	/* Switch on values from error handlers for long jump. */
	switch(permission)
	{
		case UNKNOWN: /* Page is not yet analyzed */
			
			/* Read will be attempted to current address */
			attempt = op_read;

			/* Try to read from current address */
			char * loc = (char *) address;
			char locValue = *loc;

			/* Write will be attempted to current address */
			attempt = op_write;

			/* Try to write to current address */
			*loc = 0;
			*loc = locValue;

			/* If execution has reached here, the current page is
			 * readable and writable */
			return 1;
		case NOACC:
			/* Page is not accessible */
			debug(2, "Read Denied.\n");
			return -1;
		case READ:
			/* Page has read access but no write access */
			debug(2, "Write Denied.\n");
			return 0;
	}
	/* Execution should never reach here */
	printf("ERROR: setjmp returned unexpected value!\n");
	exit(1);
}

struct memchunk newChunk(unsigned long addr, unsigned long length, int access)
{
	/* Cast address to pointer, set permissions and initial size. */
	struct memchunk new = {(void *) addr, length, access};
	return new;
}

void clearRemaining(struct memchunk *chunk_list, int size, int numChunks)
{
	/* If there are more array positions than there are chunks, set them
	 * to consistent, invalid values to avoid confusion. */
	while (numChunks < size)
	{
		numChunks++;
		chunk_list[numChunks - 1] = newChunk(0, 0, -2);
	}
}

int insertChunk(int chunkNum,
                struct memchunk chunk,
                unsigned long rightBound,
                struct memchunk *chunk_list,
                int size)
{
        /* Calculate and set chunk length. */
        chunk.length = rightBound - (unsigned long) chunk.start;

        /* Fill passed array if there is space */
        if (chunkNum <= size)
        {
            debug(1, "Chunk inserted at array index %d\n",
                  chunkNum - 1);
            chunk_list[chunkNum - 1] = chunk;

            return 0;
        }

        /* Array is already full. */
        return 1;
}

int get_mem_layout(struct memchunk *chunk_list, int size)
{
	/* Detect if 32 bit address space can be scanned - pointers must be 
	 * at least 4 bytes */
	if (sizeof(char *) < 4)
	{
		printf("WARNING: Cannot scan 32 bit memory space.\n");
		int addrBits = 8 * sizeof(char *);
		printf("         Only %d bits are addressable.\n", addrBits);

		return -1;
	}

	/* Declare and initialize variables */

	/* Cast addresses from unsigned longs, because they are >= 32 bits in
	 * size according to spec.  Size mismatch between pointer and 
	 * unsigned long doesn't matter, since we only use the lowest 32 bits */
	const unsigned long MAX_ADDR = 0xFFFFFFFFLU;;
	const unsigned long PAGE_SIZE = (unsigned long) getpagesize();
	
	unsigned long currAddr = 0LU;
	unsigned long prevAddr = 0LU;

	/* Track currrent permissions to trigger memchunk creation. */
	int currPerm = getPermission(currAddr);

	/* Initialize first memchunk, length unknown. */
	struct memchunk currChunk = newChunk(currAddr, -1, currPerm);

	int numChunks = 0;

        printf("Mapping address space...\n");

	/* To account for the possibility of overflow after checking the final
	 * page, the loop terminates if the previous address is greater than 
	 * the current address.
	 */
	while (currAddr <= MAX_ADDR && prevAddr <= currAddr)
	{
		int pagePerm = getPermission(currAddr);

		if (pagePerm != currPerm)
		{
			debug(1, "New RW=%d chunk @ %lu\n", pagePerm, currAddr);

			numChunks++;

                        /* Insert current chunk into array before replacing
                         * it with a new one. */
                        insertChunk(numChunks,
                                        currChunk,
                                        currAddr,
                                        chunk_list,
                                        size);
			
			/* Current page address is beginning of a new chunk */
			currChunk = newChunk(currAddr, -1, pagePerm);
		}

		/* Maintain loop state. */
		currPerm = pagePerm;
		prevAddr = currAddr;
		currAddr += PAGE_SIZE;
	}

        /* Increment for final chunk. */
        numChunks++;

        debug(1, "Address mapping complete.\n");

        /* Insert final chunk into array. */
        insertChunk(numChunks,
                        currChunk,
                        currAddr,
                        chunk_list,
                        size);

        debug(1, "Final permission level: %d\n", currPerm);
        debug(1, "Final address checked: %lu\n", prevAddr);

	clearRemaining(chunk_list, size, numChunks);
	
	return numChunks;
}
