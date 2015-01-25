#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>

typedef enum { false, true } bool;

jmp_buf env;

void handler(int sig)
{
    printf("in handler\n");
    int i = 1000000000;
    while (i > 0) {
        if (i % 100000000 == 0) printf("%d\n", i / 100000000);
        i--;
    }
    printf("leaving handler\n");
    siglongjmp(env, 1);
}

int main()
{
    struct sigaction act;
    bool signal_raised;
    int loop_number;
    
    /* set segfault handler */
    act.sa_handler = handler;

    /* make handler uninterruptible */
    sigfillset(&act.sa_mask);
    act.sa_flags = 0;

    /* register segfault handler */
    sigaction(SIGSEGV, &act, 0);

    loop_number = 0;

    /* set reentry point from handler */
    sigsetjmp(env, 1);

    printf("entering while(1) loop\n");

    signal_raised = false;
    while (1)
    {
        loop_number++;

        printf("loop number %d\n", loop_number);

        if (signal_raised) printf("signal raise detected\n");

        sleep(1);

        /* cause segfault every third iteration*/
        if (loop_number % 3 == 0) {
            printf("raising signal\n");
            signal_raised = true;
            raise(SIGSEGV);
        }
    }
}
