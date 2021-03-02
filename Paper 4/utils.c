#include "utils.h"

int compare_arrival_time(const void *proc_a, const void *proc_b)
{
    // order 2 processes by ascending arrival times
    // if the 2 arrival times are the same, order by ascending pid
    if (((process *)proc_a)->arrival_time == ((process *)proc_b)->arrival_time)
        return ((process *)proc_a)->pid - ((process *)proc_b)->pid;
    return (((process *)proc_a)->arrival_time - ((process *)proc_b)->arrival_time);
}

void destroy_processes(process *processes, int n)
{
    for (int i = 0; i < n; i++)
    {

        free(processes + i);
    }
}

queue *init_queue()
{
    queue *process_queue = (queue *)malloc(sizeof(queue));
    process_queue->front = NULL;
    return process_queue;
}

int is_empty(queue *process_queue)
{
    return !process_queue->front;
}

int get_queue_length(queue *process_queue)
{
    int length = 0;

    if (!is_empty(process_queue))
    {
        node *curr = process_queue->front;

        while (curr)
        {
            length++;
            curr = curr->next;
        }
    }

    return length;
}

void enqueue(queue *process_queue, process *proc)
{
    // add a new process to the end of the queue
    node *new_node = (node *)malloc(sizeof(node));
    new_node->proc = proc;
    new_node->next = NULL;

    if (is_empty(process_queue))
        process_queue->front = new_node;
    else
    {
        node *curr = process_queue->front;
        // move to the last node in queue
        while (curr->next)
        {
            curr = curr->next;
        }
        curr->next = new_node;
    }
}

process *dequeue(queue *process_queue)
{
    // remove the first process in queue and return a pointer to it
    if (!is_empty(process_queue))
    {
        node *front = process_queue->front;
        process *proc = front->proc;
        process_queue->front = front->next;
        free(front);
        return proc;
    }
    return NULL;
}

process *peek_front(queue *process_queue)
{
    // return a pointer to the first process in queue without removing it
    if (!is_empty(process_queue))
    {
        return process_queue->front->proc;
    }
    return NULL;
}

void move_front_to_end(queue *process_queue)
{
    // move the process at the front of the queue to the end
    enqueue(process_queue, dequeue(process_queue));
}

void destroy_queue(queue *process_queue)
{
    if (!is_empty(process_queue))
    {
        node *prev, *curr = process_queue->front;
        while (curr)
        {
            prev = curr;
            curr = curr->next;
            free(prev);
        }
    }
    free(process_queue);
}

void display_stats(process *processes, int n)
{
    int total_waiting_time = 0, total_turnaround_time = 0;
    printf("+-----+------------+--------------+-----------------+\n");
    printf("| PID | Burst Time | Waiting Time | Turnaround Time |\n");
    printf("+-----+------------+--------------+-----------------+\n");
    for (int i = 0; i < n; i++)
    {
        printf("| %3d | %10d | %12d | %15d |\n",
               processes[i].pid,
               processes[i].burst_time,
               processes[i].turnaround_time - processes[i].burst_time,
               processes[i].turnaround_time);
        total_waiting_time += processes[i].turnaround_time - processes[i].burst_time;
        total_turnaround_time += processes[i].turnaround_time;
    }
    printf("+-----+------------+--------------+-----------------+\n");
    printf("\n");

    printf("Average Waiting Time: %.2f\n", (double)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (double)total_turnaround_time / n);
    printf("\n");
}
