#include "utils.h"

int getNumProcesses();
int getTimeQuantum();
struct ProcessInfo *receiveAndProcessInput(int n);
void simulateESRR(int n, int time_quantum, struct ProcessInfo *processes);
void runExistingProcesses(struct ProcessQueue *queue, int *nextCpuIdleTime, int runUntil, int time_quantum);
void printStats(int n, struct ProcessInfo *processes);

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
        processes[i].pid = i;
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

    struct ProcessQueue *queue = createQueue();

    int nextCpuIdleTime = 0;
    for (int i = 0; i < n; i++)
    {
        runExistingProcesses(queue, &nextCpuIdleTime, processes[i].arrival_time, time_quantum);
        insertIntoQueue(queue, &processes[i]);
        printf("nextCpuIdleTime: %d, arrival time: %d\n", nextCpuIdleTime, processes[i].arrival_time);
    }

    runExistingProcesses(queue, &nextCpuIdleTime, INT_MAX, time_quantum);

    destroyQueue(queue);
}

void runExistingProcesses(struct ProcessQueue *queue, int *nextCpuIdleTime, int runUntil, int time_quantum)
{
    // Runs all the existing processes in the ready queue until nextCpuIdleTime >= runUntil
    // or there are no more processes in the ready queue
    while (*nextCpuIdleTime < runUntil && !isQueueEmpty(queue))
    {
        struct ProcessInfo *current = popQueueFront(queue);

        if (current->remaining_execution_time <= time_quantum)
        {
            *nextCpuIdleTime += current->remaining_execution_time;
            current->remaining_execution_time = 0;
            current->turn_around_time = *nextCpuIdleTime;
        }
        else
        {
            current->remaining_execution_time -= time_quantum;
            *nextCpuIdleTime += time_quantum;
            insertIntoQueue(queue, current);
        }
    }
}

void printStats(int n, struct ProcessInfo *processes)
{
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

    printf("Average Waiting Time: %.2f\n", (double)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (double)total_turnaround_time / n);
    printf("\n");
}