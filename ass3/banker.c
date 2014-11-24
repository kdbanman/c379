
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
#include <ctype.h>
#include <string.h>
#include "banker.h"

int in_purge()
{
        char * overrun;

        /* Read pre-newline stdin characters into overrun buffer. */
        overrun = (char *) malloc(255 * sizeof (char));
        scanf("%[^\n]%*c", overrun);

        /* Trim leading whitespace from buffer and return remaining length. */
        while(isspace(*overrun)) overrun++;
        return (int) strlen(overrun);
}

process * get_process(int num, resources * res)
{
        process * proc;

        //STUB
        num=num;
        res=res;
        proc = (process *) malloc(sizeof (process));

        /* Until valid input is given, ask for details of process. */

        return proc;
}

int get_num_processes()
{
        int num;
        char * msg;

        /* Attempt to read an integer from stdin, displaying help after an
         * invalid attempt. */
        msg = "Number of processes: ";
        num = -1;
        while (num <= 0) {
                printf(msg);
                scanf("%d", &num);
                if (in_purge()) num = 0;
                msg = "Number of processes (integer >= 1): ";
        }

        return num;
}

proc_list * get_process_list(resources * res)
{
        proc_list * head, * current, * next;
        int count, to_get;

        /* Manage number of processes. */
        count = get_num_processes();

        /* Get the process for the head of the linked list. */
        head = (proc_list *) malloc(sizeof (proc_list));
        head->proc = get_process(0, res);
        head->next = NULL;

        /* Get the processes for the tail of the linked list. */
        current = head;
        to_get = 1;
        while (to_get < count) {
                /* Get a new process. */
                next = (proc_list *) malloc(sizeof (proc_list));
                next->proc = get_process(to_get, res);
                next->next = NULL;

                /* Link the new process to the existing one. */
                current->next = next;

                /* Move to the end of the list. */
                current = next;

                to_get++;
        }

        return head;
}

int get_num_resources()
{
        int num;
        char * msg;

        /* Attempt to read an integer from stdin, displaying help after an
         * invalid attempt. */
        msg = "Number of resource types: ";
        num = -1;
        while (num <= 0) {
                printf(msg);
                scanf("%d", &num);
                if (in_purge()) num = 0;
                msg = "Number of resource types (integer >= 1): ";
        }

        return num;
}

char ** get_resource_names(int num_res)
{
        char ** names;
        int i;

        names = (char **) malloc(sizeof (char *) * num_res);

        for (i = 0; i < num_res; i++) {
                /* Prepare for resource names up to 255 characters. */
                names[i] = (char *) malloc(sizeof (char) * 255);
        }

        /* Take input until 3 whitespace-separated strings are entered. */
        /* If a name longer than 255 characters is entered, it is read as 
         * more than 1 name.  This guards against buffer overrun attack. */
        printf("Names of resource types: ");
        for (i = 0; i < num_res - 1; i++) {
                scanf("%255s ", names[i]);
        }
        scanf("%255s", names[i]);
        in_purge();

        return names;
}

int * get_max_resources(int num_res)
{
        int i, valid, * max;
        char * msg;

        max = (int *) malloc(sizeof (int) * num_res);

        for (i = 0; i < num_res; i++) max[i] = -1;

        /* Until input is valid, read space-separated integers from stdin. */
        msg = "Number of instances of each resource type%d\b: ";
        valid = 0;
        while (!valid) {
                printf(msg, num_res);

                /* Attempt to read the appropriate number of integers. */
                for (i = 0; i < num_res - 1; i++) {
                        scanf("%d ", &max[i]);
                }
                scanf("%d", &max[i]);
                if (in_purge()) max[0] = -1;

                /* If all elements are set properly, set "valid" flag. */
                for (i = 0; i < num_res; i++) {
                        if (max[i] <= 0) {
                                valid = 0;
                                break;
                        } else {
                                valid = 1;
                        }
                }
                msg = "Number of each resource type (%d integers >= 1): ";
        }

        return max;
}

resources * get_resources()
{
        resources * res;

        res = (resources *) malloc(sizeof (resources));

        /* Get each struct field. */
        res->num = get_num_resources();
        res->name = get_resource_names(res->num);
        res->max = get_max_resources(res->num);

        /* Copy max available resources to currently available resources. */
        res->avail = (int *) malloc(sizeof (int) * res->num);
        memcpy(res->avail, res->max, sizeof (int) * res->num);

        return res;
}

simulation * get_simulation()
{
        simulation * sim;

        sim = (simulation *) malloc(sizeof (simulation));

        sim->time = 0;
        sim->res = get_resources();
        sim->procs = get_process_list(sim->res);

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
        head = NULL;

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

        printf("Simulation end.\n");

        return 0;
}
