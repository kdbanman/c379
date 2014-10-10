#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int i;
    for (i = 0; i < 4; i++) {
        fork();
        wait(NULL);
        printf("process %d i%d\n", getpid(), i);
    }

    return 0;
}
