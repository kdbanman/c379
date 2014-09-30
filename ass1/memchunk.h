/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 1
 */

/* Required functionality. */

struct memchunk {
	void *start;
	unsigned long length;
	int RW;
};

int get_mem_layout(struct memchunk *chunk_list, int size);

/* Support functionality. */

typedef enum { op_read, op_write } operation;

void debug(int debugLevel, const char * format, ...);

void rw_err_handler(int sig);

int getPermission(unsigned long address);

struct memchunk newChunk(unsigned long addr, unsigned long length, int access);

void clearRemaining(struct memchunk *chunk_list, int size, int numChunks);
