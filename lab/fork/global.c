#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int value = 5;

int main()
{
    pid_t pid;
    while (value > 0) {
        value -= 2;
        pid = fork();
        if (pid == 0) {
            value++;
            printf("child %d: value = %d\n", getpid(), value);
        } else {
            wait(NULL);
            printf("parent: value = %d\n", value);
        }
    }
    return 0;
}
