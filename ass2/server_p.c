
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include <stdio.h>
#include "server_util.h"
#include "server_f.h"

int main()
{
        struct serverconf config;

        config = getConfig(argc, argv);

        printf("%d %s %s\n", config.port, config.basedir, config.logloc);

        return 0;
}
