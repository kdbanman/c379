
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "banker.h"

simulation * get_simulation()
{
        simulation * sim;

        //STUB
        sim = (simulation *) malloc(sizeof (simulation));

        return sim;
}

void log_events(evt_list * evts)
{
        //STUB
        evts = evts;
        printf("sim doin stuff\n");
}

void log_result(simulation * sim)
{
        //STUB
        sim = sim;
        printf("sim did stuff\n");
}

evt_list * run_simulation(simulation * sim)
{
        evt_list * head;

        //STUB
        sim = sim;
        head = (evt_list *) malloc(sizeof (evt_list));

        return head;
}

int main()
{
        simulation * sim;
        evt_list * evts;

        /* Gather and verify input from user. */
        sim = get_simulation();

        printf("Simulation start.\n");

        /* Run simulation and output simulation events. */
        evts = run_simulation(sim);
        while (evts != NULL) {
                log_events(evts);
                evts = run_simulation(sim);
        }

        /* Output simulation results. */
        log_result(sim);

        printf("Simulation end.");

        return 0;
}
