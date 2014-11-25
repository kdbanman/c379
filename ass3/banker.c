
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
        while (isspace(*overrun)) overrun++;
        return (int) strlen(overrun);
}

char * get_process_name()
{
        char * name;

        /* Read up to whitespace. */
        name = (char *) malloc(255 * sizeof (char));
        scanf("%s ", name);

        /* Trim leading whitespace. */
        while (isspace(*name)) name++;
        return name;
}

int * get_process_needs(resources * res)
{
        int i, * need;

        need = (int *) malloc(res->num * sizeof(int));

        /* Attempt to read the appropriate number of integers. */
        for (i = 0; i < res->num; i++) {
                need[i] = -1;
                scanf("%d ", &need[i]);

                if (need[i] < 0 || need[i] > res->max[i]) return NULL;
        }

        return need;
}

process * get_process(int num, resources * res)
{
        process * proc;
        int valid;
        char * msg;

        proc = (process *) malloc(sizeof (process));
        proc->start = -1;
        proc->dur = -1;
        proc->remaining = -1;
        proc->state = INERT;

        /* Until valid input is given, ask for details of process. */
        msg = "Details of process %d: ";
        valid = 0;
        while (!valid) {
                printf(msg, num);
                proc->name = get_process_name();
                proc->need = get_process_needs(res);
                scanf("%d ", &(proc->start));
                scanf("%d", &(proc->dur));

                /* If purge is not empty, set erroneous value. */
                if (in_purge()) proc->dur = -1;

                /* If any values are set erroneously, do not set valid flag. */
                if (proc->need != NULL &&
                    proc->start >= 0 &&
                    proc->dur > 0) valid = 1;

                msg = "Details of process %d (name needs start duration): ";
        }
        proc->remaining = proc->dur;

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
                printf("%s", msg);
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
                printf("%s", msg);
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

                /* If nonempty purge, set erroneous value. */
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

        /* Read simulation config from stdin. */
        sim->time = 0;
        sim->res = get_resources();
        sim->procs = get_process_list(sim->res);

        return sim;
}

void append_event(int time, process * p, evt_code code, evt_list ** evts)
{
        evt_list * next, * new;

        /* Create the new event list element. */
        new = (evt_list *) malloc(sizeof (evt_list));
        new->evt = (event *) malloc(sizeof (event));
        new->evt->time = time;
        new->evt->event = code;
        new->evt->proc = p->name;
        new->next = NULL;

        /* If the list is empty, set it's head to the new element. */
        if (*evts == NULL) {
                *evts = new;
        }else {
            /* Find the end of the list. */
            next = *evts;
            while (next->next != NULL) next = next->next;
            
            /* Append the new element. */
            next->next = new;
        }

}

void log_event(event * evt)
{
        char * msg;

        if (evt->event == ARRIVE) 
                msg = "Process %s arrived.\n";
        else if (evt->event == START)
                msg = "Process %s started execution.\n";
        else if (evt->event == FINISH)
                msg = "Process %s finished execution.\n";

        printf(msg, evt->proc);
}

void log_events(evt_list * evts)
{
        evt_list * next;
        event * evt;

        /* Report the time from the first event. */
        printf("\nSimulation time: %d\n", evts->evt->time);

        /* Iterate through the list of events. */
        next = evts;
        while (next != NULL) {
                /* Get the event. */
                evt = next->evt;

                /* Log event contents. */
                log_event(evt);

                next = next->next;
        }
}

void log_process(process * proc)
{
        /* If process is running, but has just arrived, do not log. */
        if (proc->state == RUNNING && proc->dur != proc->remaining + 1) {
                printf("Process %s is running.\n", proc->name);
        } else if (proc->state == WAITING) {
                printf("Process %s is waiting for resources.\n", proc->name);
        }

}

void log_processes(proc_list * procs)
{
        proc_list * next;

        /* Iterate through process list. */
        next = procs;
        while (next != NULL) {
                log_process(next->proc);
                next = next->next;
        }
}

int consume_resources(process * p, resources * res)
{
        int i;

        /* Check if resources are consumable, return 1 if not. */
        for (i = 0; i < res->num; i++) {
                if (p->need[i] > res->avail[i]) return 1;
        }

        /* Subtract process needs from available resources. */
        for (i = 0; i < res->num; i++) {
                 res->avail[i] -= p->need[i];
        }

        return 0;
}

void release_resources(process * p, resources * res)
{
        int i;

        /* Add process needs to available resources. */
        for (i = 0; i < res->num; i++) {
                 res->avail[i] += p->need[i];
        }
}

void update_by_process(int time, process * p, resources * res, evt_list ** evts)
{
        if (p->start == time) {
                /* Create and append arrival event. */
                append_event(time, p, ARRIVE, evts);
        }

        /* Check if process is new or waiting on resources. */
        if (p->start == time || p->state == WAITING) {
                if (consume_resources(p, res)) {
                        /* Resource consumption failed, tell process to wait. */
                        p->state = WAITING;
                } else {
                        /* Resources allocated, set to running. */
                        p->state = RUNNING;

                        /* Create and append start event. */
                        append_event(time, p, START, evts);
                }
        }

        if (p->state == RUNNING) {
                /* Check if there is remaining time left. */
                if (p->remaining > 0) {
                        /* Process has completed an iteration, so decrement
                         * remaining duration. */
                        p->remaining--;
                } else {
                        p->state = DONE;
                        release_resources(p, res);
                        /* Create and append finish event. */
                        append_event(time, p, FINISH, evts);
                }
        }
}

void update_simulation(simulation * sim, evt_list ** evts)
{
        proc_list * next;

        next = sim->procs;

        /* Iterate through linked list of processes. */
        while (next != NULL) {
                /* Update according to the current process, gathering events. */
                update_by_process(sim->time, next->proc, sim->res, evts);

                /* Get the next process in the list. */
                next = next->next;
        }
}

int simulation_complete(simulation * sim)
{
        proc_list * next;
        int done;

        /* Iterate through linked list of processes. */
        done = 1;
        next = sim->procs;
        while (next != NULL) {
                /* Simulation is incomplete if any process is either:
                 * - INERT (not yet arrived)
                 * - RUNNING
                 */
                if (next->proc->state == INERT ||
                    next->proc->state == RUNNING) done = 0;

                next = next->next;
        }

        return done;
}

int run_simulation(simulation * sim)
{
        evt_list * evts;

        /* Run simulation until events are encountered. */
        evts = NULL;
        while (evts == NULL) {
            update_simulation(sim, &evts);
            sim->time++;
        }

        log_events(evts);
        log_processes(sim->procs);

        return simulation_complete(sim);
}

void log_result(simulation * sim)
{
        proc_list * next;
        int waiting;
        char * msg;

        printf("\nNo process running.\n");

        /* Iterate through process list to find any waiting process. */
        next = sim->procs;
        waiting = 0;
        while (next != NULL) {
                if (next->proc->state == WAITING) waiting = 1;
                next = next->next;
        }

        if (waiting) {
                msg = "Process(es) waiting.\n"
                      "Insufficient resources to avoid deadlocks.\n";
        } else 
                msg = "All processes finished.\n";
        
        printf("%sSystem halted.\n", msg);
}

int main()
{
        simulation * sim;
        int done;

        /* Gather and verify input from user. */
        sim = get_simulation();

        printf("\n\nSimulation time: 0\n");
        printf("Simulation started.\n");
        printf("No process is available.\n");

        /* Run simulation. */
        done = 0;
        while (!done) {
                done = run_simulation(sim);
        }

        /* Output simulation results. */
        log_result(sim);

        printf("Simulation end.\n\n");

        return 0;
}
