
/*
 * Kirby Banman
 * 1173895
 *
 * CMPUT 379 EA1
 * Assignment 3
 *
 */

#ifndef BANKER_H_
#define BANKER_H_

/******************
 *                *
 *  DATA SECTION  *
 *                *
 ******************/

/* Event codes. */
typedef enum {ARRIVE, START, FINISH} evt_code;

/* Process states. */
typedef enum {INERT, WAITING, RUNNING, DONE} proc_state;

/*
 * Structure for tracking simulation resources.  In order of declaration:
 *   - number of different types of resources
 *   - name of each resource
 *   - maximum number of each type of resource the system can allocate
 *   - currently available number of each type of resource
 */
typedef struct resources {
        int num;
        char ** name;
        int * max;
        int * avail;
} resources;

/*
 * Structure for a single process for the simulation.  In order of declaration:
 *   - name of process
 *   - number of resources required for each resource type
 *   - arrival (resource request) time
 *   - time required for execution
 */
typedef struct process {
        char * name;
        int * need;
        int start;
        int dur;
        proc_state state;
} process;

/*
 * Linked list item for simulation processes.  In order of declaration:
 *  - list item payload
 *  - next list item
 */
typedef struct proc_list_struct {
        process * proc;
        struct proc_list_struct * next;
} proc_list;

/*
 * Structure for a simulation event involving a process.  Events include:
 *   1. Process arrival
 *   2. Process execution start
 *   3. Process execution finish
 * In order of declaration:
 *   - simulation time event occurrence
 *   - event code (see list above)
 *   - process name
 */
typedef struct event {
        int time;
        evt_code event;
        char * proc;
} event;

/* Linked list item for concurrent events. In order of declaration:
 *  - time of each event in list
 *  - list item payload
 *  - next list item
 */
typedef struct evt_list_struct {
        int time;
        event * evt;
        struct evt_list_struct * next;
} evt_list;

/*
 * Data required for simulation.  Each field will be mutated as simulation
 * progresses.  In order of declaration:
 *  - current simulation time
 *  - current system resources
 *  - linked list of processes
 */
typedef struct simulation {
        int time;
        resources * res;
        proc_list * procs;
} simulation;

#endif

/**********************
 *                    *
 *  FUNCTION SECTION  *
 *                    *
 **********************/

/********************
 * CONFIG FUNCTIONS *
 ********************/

/*
 * Propmts users to input simulation data from stdin, verifying correctness
 * and consistency.  If incorrect or inconsistent input is supplied, the user
 * is notified and the program continues if possible.
 *
 * @return The ready-to-run simulation.
 */
simulation * get_simulation();

/********************
 * OUTPUT FUNCTIONS *
 ********************/

/*
 * Outputs simulation time and each event in the list as a line on stdout.
 */
void log_events(evt_list * evts);

/*
 * Outputs simulation result to stdout.  Assumes simulation has been run to
 * completion by iterative calls to run_simulation.  Two results possible:
 *  - All processes completed successfully.
 *  - One or more processes could not finish.
 */
void log_result(simulation * sim);

/************************
 * SIMULATION FUNCTIONS *
 ************************/

/*
 * Runs (and mutates) the simulation until at least one event occurs.
 *
 * @param sim The simulation to be run and mutated.
 * @return The list of events, or null if the simulation cannot proceed.
 */
evt_list * run_simulation(simulation * sim);
