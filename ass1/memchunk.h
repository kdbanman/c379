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

int get_mem_layout(struct memchunk *chunk_list, int size);

void read_err_handler(int sig);
void write_err_handler(int sig);
