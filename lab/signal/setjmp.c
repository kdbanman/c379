#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <unistd.h>

jmp_buf env;

void handler(int sig)
{
    printf("signal received: %d\n", sig);

    siglongjmp(env, 1);
}

int main()
{
    /* config and register segfault handler */
    struct sigaction act;
    
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGSEGV, &act, 0);

    /* put some state on the stack and save it to env */
    int testState = 0;
    sigsetjmp(env, 1);

    /* 
     * ENVIRONMENT SHOULD BE RESTORED, SO testState SHOULD ALWAYS BE 0, BUT
     * IT ISN'T.  WHY?
     */
    printf("test state %d\n", testState);

    /* change stack state.
     *
     * THIS CHANGE SHOULD DISAPPEAR AFTER siglongjmp IS 
     * CALLED, BUT IT'S PERSISTING.  WHY?
     */
    testState = 1;

    /* raise signal after a second */
    printf("raising signal...\n");
    sleep(1);
    raise(SIGSEGV);

}
