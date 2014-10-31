#include <stdio.h>
#include <stdlib.h>

char * test(char * balls)
{
    char * ass = (char *) malloc(800 * sizeof ' ');

    balls = "pooooooop";

    sprintf(ass, "%d bitch asssssssssssssssssssss %s", 838838383, balls);

    return ass;
}

int main()
{
    char * meat;

    puts(meat);

    char * ploo = test(meat);
    puts(ploo);
    free(&ploo[0]);

    puts(meat);

    int a[1000];
    int i = 0;
    for (;i<1000;i++) a[i] = i;

    puts(meat);

    return 0;
}
