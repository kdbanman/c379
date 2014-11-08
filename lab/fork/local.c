#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    char * s = (char *) malloc(500);

    sprintf(s, "%s", "parent");

    pid_t pid = fork();
    if (pid == 0) {
        printf("child: %s\n", s);

        sprintf(s, "%s", "child");

        printf("child: %s\n", s);
    } else {
        wait(NULL);

        pid = fork();
        if (pid == 0)
            printf("child2: %s\n", s);
        else
            printf("parent: %s\n", s);
    }
    return 0;
}
