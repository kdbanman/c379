#include <signal.h>
#include <stdio.h>
#include <unistd.h>

typedef enum { false, true } bool;

bool err;

void ouch(int sig)
{
    printf("signal: %d\n", sig);
    err = true;
}

int main()
{
    err = false;

    struct sigaction act;

    act.sa_handler = ouch;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, 0);

    while (1)
    {
        if (!err) {
            printf("nonblock?\n");
        } else {
            printf("nonblock? sigint recieved\n");
        }
        sleep(1);
    }
}
