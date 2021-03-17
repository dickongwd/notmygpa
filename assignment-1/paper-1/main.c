#include "utils.h"

int main(void)
{
    int n = getNumProcesses();
    int time_quantum = getTimeQuantum();
    struct ProcessInfo *processes = receiveAndProcessInput(n);
    simulateESRR(n, time_quantum, processes);
    printStats(n, processes);
    return 0;
}

int getNumProcesses()
{
    int n;
    printf("Enter number of processes:\n");
    scanf("%d", &n);
    return n;
}

int getTimeQuantum()
{
    int tq;
    printf("Enter time quantum:\n");
    scanf("%d", &tq);
    return tq;
}

struct ProcessInfo *receiveAndProcessInput(int n)
{
    struct ProcessInfo *processes = malloc(n * sizeof(struct ProcessInfo));
    for (int i = 0; i < n; i++)
    {
        processes[i].pid = i + 1;
    }
    printf("Enter burst times of all processes (separated by a space):\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &processes[i].initial_execution_time);
        processes[i].remaining_execution_time = processes[i].initial_execution_time;
    }
    printf("Enter arrival times for all processes (separated by a space):\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &processes[i].arrival_time);
    }

    return processes;
}

void simulateESRR(int n, int time_quantum, struct ProcessInfo *processes)
{
    qsort(processes, n, sizeof(struct ProcessInfo), arrivalTimeComparator);

    struct EventHeap *events = createPriorityQueue(MAX_NUM_EVENTS);
    struct ProcessQueue *queue = createQueue();
    int is_cpu_idle = 1;
    int process_arrived = 0;

    for (int i = 0; i < n; i++)
        insertEvent(events, processes[i].arrival_time, PROCESS_ARRIVE, &processes[i]);

    while (!isPriorityQueueEmpty(events))
    {
        struct Event current_event = *getEarliestEvent(events);
        popEarliestEvent(events);
        printEventStatus(&current_event, queue, 0);

        switch (current_event.event_type)
        {
        case PROCESS_ARRIVE:
            sortQueue(queue);
            if (is_cpu_idle)
            {
                is_cpu_idle = 0;
                runNextProcess(events, current_event.pinfo, time_quantum, current_event.time);
            }
            else
                insertQueueBack(queue, current_event.pinfo);

            process_arrived = 1;
            break;
        case TIME_QUANTUM_END:
            if (current_event.pinfo->remaining_execution_time == 0)
            {
                if (isQueueEmpty(queue))
                {
                    is_cpu_idle = 1;
                }
                else
                {
                    if (process_arrived)
                        sortQueue(queue);
                    runNextProcess(events, popQueueFront(queue), time_quantum, current_event.time);
                }
                current_event.pinfo->turn_around_time = current_event.time - current_event.pinfo->arrival_time;
            }
            else
            {
                insertQueueBack(queue, current_event.pinfo);
                if (process_arrived)
                    sortQueue(queue);
                runNextProcess(events, popQueueFront(queue), time_quantum, current_event.time);
            }

            process_arrived = 0;
            break;
        }

        printEventStatus(&current_event, queue, 1);
    }

    destroyQueue(queue);
    destroyPriorityQueue(events);
}

void runNextProcess(struct EventHeap *events, struct ProcessInfo *next_process, int time_quantum, int current_time)
{
    int next_cycle_length;
    if (next_process->remaining_execution_time <= time_quantum)
    {
        next_cycle_length = next_process->remaining_execution_time;
        next_process->remaining_execution_time = 0;
    }
    else
    {
        next_cycle_length = time_quantum;
        next_process->remaining_execution_time -= time_quantum;
    }
    insertEvent(events, current_time + next_cycle_length, TIME_QUANTUM_END, next_process);
}

void printEventStatus(struct Event *event, struct ProcessQueue *queue, int is_event_handled)
{
    char *event_names[] = {"PROCESS ARRIVE", "TIME QUANTUM END"};
    if (!is_event_handled)
    {
        printf("Time: %d\n", event->time);
        printf("%s: Process %d\n", event_names[event->event_type], event->pinfo->pid);
    }
    else
        printf("After handling:\n");
    printf("Processes in queue: ");
    printQueue(queue);

    if (is_event_handled)
        printf("\n");
}

void printStats(int n, struct ProcessInfo *processes)
{
    qsort(processes, n, sizeof(struct ProcessInfo), pidComparator);
    int total_waiting_time = 0, total_turnaround_time = 0;
    printf("+-----+------------+--------------+-----------------+\n");
    printf("| No. | Burst Time | Waiting Time | Turnaround Time |\n");
    printf("+-----+------------+--------------+-----------------+\n");
    for (int i = 0; i < n; ++i)
    {
        printf("| %3d | %10d | %12d | %15d |\n",
               processes[i].pid,
               processes[i].initial_execution_time,
               processes[i].turn_around_time - processes[i].initial_execution_time,
               processes[i].turn_around_time);
        total_waiting_time += processes[i].turn_around_time - processes[i].initial_execution_time;
        total_turnaround_time += processes[i].turn_around_time;
    }
    printf("+-----+------------+--------------+-----------------+\n");
    printf("\n");

    printf("Average Turnaround Time: %.2f\n", (double)total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", (double)total_waiting_time / n);
    printf("\n");
}

int arrivalTimeComparator(const void *proc1, const void *proc2)
{
    return ((struct ProcessInfo *)proc1)->arrival_time - ((struct ProcessInfo *)proc2)->arrival_time;
}

int pidComparator(const void *proc1, const void *proc2)
{
    return ((struct ProcessInfo *)proc1)->pid - ((struct ProcessInfo *)proc2)->pid;
}
