#ifndef UTIL_H
#define UTIL_H

extern const int MAX_NUM_PROCESS;

/*
 * Process to represent all data items for each process
 */
typedef struct
{
    int id;     // Process number
    int bt;     // Burst time
    int btleft; // Burst time left
    int at;     // Arrival time
    int p;      // Priority
    int tq;     // Time Quantum
    bool done;  // Flag to denote if process is done
    int tat;    // Turnaround time
    int wt;     // Waiting time
} Process;

/*
 * Node for a linked list to represent list of tasks
 */
typedef struct Task
{
    int id;            // Process number
    int st;            // Starting time for task
    int et;            // Ending time for task
    struct Task *next; // Pointer to next task
} Task;

/*
 * Handles process next in the queue.
 * 
 * @param p     Process next in the queue
 * @param ct    Current time from start
 * @param npd   Number of processes done
 */
void handleProcess(Process *p, int *ct, int *npd);

/*
 * Completes execution for the selected process
 *      Sets burst time left to 0
 *      Sets done flag to true
 *      Calculates turnaround time
 *      Calculates waiting time 
 * 
 * @param p     Process to execute
 * @param ct    Current time from start
 */
void completeProcess(Process *p, int ct);

/*
 * Appends task to linked list. Used for final Gantt chart.
 * 
 * @param head  Pointer to head of linked list
 * @param p     Process number
 * @param st    Time start for task
 * @param et    Time end for task
 */
void appendTask(Task **head, int p, int st, int et);

/*
 * Calculates average waiting time and average turnaround time. Prints
 * individual waiting and turnaround times, followed by their averages,
 * and the equivalent Gantt chart for the algorithm.
 * 
 * @param n     Number of processes
 * @param p     Array of processes
 * @param t     Array of tasks
 */
void displayStats(const int n, const Process p[]);

/*
 * Prints gantt chart.
 * 
 * @param head  Pointer to task representing the start of task sequence
 */
void displayGanttChart(Task *head);

/*
 * Gets number of processes from user.
 * 
 * @return      Number of processes
 */
int getNumProcess();

/*
 * Gets time quantum from user.
 * 
 * @return      Time Quantum
 */
int getTimeQuantum();

/*
 * Gets the burst times for all processes from user.
 * 
 * @param n     Number of processes to read
 * @param p     Array of processes to be written to
 */
void getBurstTime(int n, Process p[]);

/*
 * Gets the arrival times for all processes from user.
 * 
 * @param n     Number of processes to read
 * @param p     Array of processes to be written to
 */
void getArrivalTime(int n, Process p[]);

/*
 * Gets the priority for all processes from user.
 * Priority will only be 1, 2, or 3, corresponding to low, medium and high
 * priority.
 * 
 * @param n     Number of processes to read
 * @param p     Array of processes to be written to
 */
void getPriority(int n, Process p[]);

/*
 * Sets time quantum for each process based on its priority.
 * 
 * @param n     Number of processes to set
 * @param p     Array of processes to set
 * @param tq    Given time quantum
 */
void setTimeQuantum(int n, Process p[], int tq);

/*
 * Gets the greatest common divisor of two integers.
 * Used as a helper function for the creation of the Gantt chart.
 * 
 * @param a     First integer
 * @param b     Second integer
 * 
 * @return      gcd of a and b
 */
int gcd(int a, int b);

#endif
