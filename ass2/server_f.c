
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 2
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "server_conf.h"
#include "server_util.h"
#include "log_monitor.h"
#include "server_f.h"

int main(int argc, char *argv[])
{
        serverconf config;
        int sd;
        safefile log;

        config = getConfig(argc, argv);

        sd = listeningSock(config);

        printf("Serving from %s on port %d, logging to %s\n",
               config.basedir,
               config.port,
               config.logloc);

        while (1) {
                processClient(sd, config, &log);
        }

        close(sd);

        return 0;
}
