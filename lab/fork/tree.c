#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int value = 0;
int main()
{
    printf("process %d value %d\n", getpid(), value);

    fork();
    value++;
    printf("process %d value %d\n", getpid(), value);

    fork();
    value++;
    printf("process %d value %d\n", getpid(), value);

    fork();
    value++;
    printf("process %d value %d\n", getpid(), value);

    return 0;
}
