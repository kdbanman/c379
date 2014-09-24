/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 1
 */


struct memchunk {
    void *start;
    unsigned long length;
    int RW;
};

typedef enum { op_read, op_write } operation;

int get_mem_layout(struct memchunk *chunk_list, int size);

