#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>

typedef enum { false, true } bool;

bool err;

jmp_buf env;

void handler(int sig)
{
    printf("signal received: %d\n", sig);
    err = true;

    siglongjmp(env, 1);
    
    printf("here\n");
}

int main()
{
    /* config and register segfault handler */
    struct sigaction act;
    
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGSEGV, &act, 0);

    err = false;

    sigsetjmp(env, 1);

    printf("post setjmp\n");

    int i = 0;
    while (1)
    {
        i++;

        if (err) printf("err!!\n");
        else printf("noerr\n");

        err = false;

        sleep(1);

        /* cause segfault every third iteration*/
        if (i % 3 == 0) {
            printf("raising signal\n");
            raise(SIGSEGV);
        }
    }
}
