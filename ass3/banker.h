
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

#define DEBUG 1

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
 *  - list item payload
 *  - next list item
 */
typedef struct evt_list_struct {
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
 * Purge stdin of previous entry, if it exists.
 *
 * @return The number of non-whitespace characters left in stdin.
 */
int in_purge();

/*
 * From stdin, read the name from a process detail input report, trimming
 * all whitespace.
 */
char * get_process_name();

/*
 * From stdin, read the process resource requirements, validating them
 * against the max available in the resources parameter.  Returns NULL if
 * any resource needs are < 0 or > max
 *
 * @param res The max system resources.
 * @return The array of resource requirements or NULL.
 */
int * get_process_needs();

/*
 * From stdin, get the details of a single process.  Returns only after valid
 * input.  Input is validated against the resources parameter.
 *
 * @param num The process number.
 * @param res The simulation resources.
 * @return A valid process struct.
 */
process * get_process(int num, resources * res);

/*
 * From stdin, get the number of processes.  Asks repeatedly until >= 1.
 */
int get_num_processes();

/*
 * From stdin, get the number of processes, as well as the properties of each
 * process.  Returns only after sufficient valid input given.  Input is 
 * validated against the resources structure parameter.
 *
 * @param res The simulation system resources
 * @return The list of processes.
 */
proc_list * get_process_list(resources * res);

/*
 * From stdin, get the number of types of system resourcs.  Must be >= 1.
 */
int get_num_resources();

/*
 * From stdin, get the name of each resource type.  Must match the number of
 * resource types.
 *
 * @param num_res The number of resource types.
 */
char ** get_resource_names(int num_res);

/*
 * From stdin, get the number of each resource type.  Must match the number of
 * resource types.
 *
 * @param num_res The number of resource types.
 */
int * get_max_resources(int num_res);

/*
 * From stdin, get the number, names, and amounts of simulation system
 * resources.  Returns only after sufficient valid input given.
 *
 * @return The resources struct
 */
resources * get_resources();

/* 
 * Prints the data for a simulation struct.
 */
void print_sim(simulation * sim);

/*
 * Propmts users to input simulation data from stdin, verifying correctness
 * and consistency.  If incorrect or inconsistent input is supplied, the user
 * is notified and the program continues if possible.
 *
 * @return The ready-to-run simulation.
 */
simulation * get_simulation();

/*******************
 * EVENT FUNCTIONS *
 *******************/

/* 
 * Appends an event for the supplied process and event code to the event list.
 */
void append_event(int time, process * p, evt_code code, evt_list ** evts);

/*
 * Logs a single event to stdout.
 */
void log_event(event * evt);

/*
 * Outputs simulation time and each event in the list as a line on stdout.
 */
void log_events(evt_list * evts);

/************************
 * SIMULATION FUNCTIONS *
 ************************/

/*
 * Attempt to consume resources for process, return nonzero if resources
 * could not be allocated (and are unchanged).
 */
int consume_resources(process * p, resources * res);

/* 
 * Release resources from process.
 */
void release_resources(process * p, resources * res);

/*
 * Update the process and the resources, appending events to the list as
 * necessary.
 */
void update_by_process(int time, process * p, resources * res, evt_list ** evt);

/*
 * For a single simulation moment, check simulation processes and update
 * resources and processes accordingly. Append any events to the event list.
 *
 * @param sim The simulation with time incremented since last update.
 * @param evts The linked list of events.
 */
void update_simulation(simulation * sim, evt_list ** evts);

/*
 * Iterates through processes, returns true if none are left running.  This
 * indicates either an unservable set of processes or all processes complete.
 *
 * @return Nonzero if simulation complete
 */
int simulation_complete(simulation * sim);

/*
 * Runs (and mutates) the simulation until at least one event occurs.
 *
 * @param sim The simulation to be run and mutated.
 * @return Nonzero if the simulation is finished
 */
int run_simulation(simulation * sim);

/*
 * Outputs simulation result to stdout.  Assumes simulation has been run to
 * completion by iterative calls to run_simulation.  Two results possible:
 *  - All processes completed successfully.
 *  - One or more processes could not finish.
 */
void log_result(simulation * sim);
