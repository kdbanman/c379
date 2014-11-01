#include <stdio.h>
#include <stdlib.h>

typedef struct thing {
    int test;
} thing;

thing * get(int a)
{
    thing * ret;
    ret = (thing *) malloc(sizeof (thing));

    ret->test = a;

    return ret;
}

int main()
{
    thing * mall = get(4);

    printf("%d\n", (*mall).test);

    free(mall);

    return 0;
}
