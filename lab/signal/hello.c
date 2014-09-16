#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ouch(int sig)
{
    printf("signal: %d\n", sig);
}

int main()
{
    int i;
    for (i = 0; i < 10000; i++) {
        (void) signal(i, ouch);
    }

    while (1)
    {
        printf("text\n");
        sleep(1);
    }
}
