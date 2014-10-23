#include <stdio.h>
#include <time.h>

int main() {

    time_t t;
    time(&t);

/*
    struct tm * ptm;
    ptm = gmtime(&t);
    */

    printf("%s\n", asctime(gmtime(&t)));

    return 0;
}
