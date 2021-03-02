#include "utils.h"

int main(void)
{
    int n = get_num_processes();
    process *processes = get_processes_info(n);

    // sort processes by arrival time
    qsort(processes, n, sizeof(process), compare_arrival_time);

    queue *process_queue = init_queue();

    printf("\nRunning simulation...\n\n");
    simulate_edrr(process_queue, processes, n);

    display_stats(processes, n);

    destroy_queue(process_queue);
    destroy_processes(processes, n);

    return 0;
}

int get_num_processes()
{
    int n;
    printf("Enter the number of processes:\n");
    scanf("%d", &n);
    return n;
}

process *get_processes_info(int n)
{
    process *processes = (process *)malloc(n * sizeof(process));
    for (int i = 0; i < n; i++)
    {
        processes[i].pid = i;
        processes[i].turnaround_time = -1;
    }
    printf("Enter burst times of all processes (separated by a space):\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }
    printf("Enter arrival times for all processes (separated by a space):\n");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &processes[i].arrival_time);
    }
    return processes;
}

int get_max_burst_time(queue *process_queue)
{
    int bt_max = 0;
    node *curr = process_queue->front;
    while (curr)
    {
        if (curr->proc->burst_time > bt_max)
            bt_max = curr->proc->burst_time;
        curr = curr->next;
    }
    return bt_max;
}

int get_time_quantum(queue *process_queue)
{

    return get_max_burst_time(process_queue) / 5 * 4;
}

void do_process(queue *process_queue, int *cpu_time, int *time_quantum)
{
    process *proc = dequeue(process_queue);

    if (proc->remaining_time <= *time_quantum)
    {
        *cpu_time += proc->remaining_time;
        proc->turnaround_time = *cpu_time;
        proc->remaining_time = 0;
    }
    else
    {
        proc->remaining_time -= *time_quantum;
        enqueue(process_queue, proc);
        *cpu_time += *time_quantum;
    }

    *time_quantum = 0;
}

void simulate_edrr(queue *process_queue, process *processes, int n)
{
    int cpu_time = 0, next_to_arrive;

    // fill up initial process queue at t = 0
    for (next_to_arrive = 0; next_to_arrive < n; next_to_arrive++)
    {
        if (processes[next_to_arrive].arrival_time <= cpu_time)
        {
            enqueue(process_queue, processes + next_to_arrive);
        }
        else
        {
            break;
        }
    }

    int time_quantum = get_time_quantum(process_queue), num_proc_remaining = 0, queue_length;

    while ((queue_length = get_queue_length(process_queue)) > 0)
    {
        if (queue_length > num_proc_remaining)
        {
            process *next_proc = peek_front(process_queue);

            if (queue_length == 1 || next_proc->burst_time <= time_quantum)
            {
                do
                    do_process(process_queue, &cpu_time, &time_quantum);
                while (time_quantum);
                time_quantum = get_time_quantum(process_queue);
            }
            else
            {
                move_front_to_end(process_queue);
                num_proc_remaining++;
            }
        }
        else
        {
            time_quantum = get_max_burst_time(process_queue);
            num_proc_remaining = 0;
        }

        // add arrived processes to queue
        while (next_to_arrive < n)
        {
            if (processes[next_to_arrive].arrival_time <= cpu_time)
            {
                enqueue(process_queue, processes + next_to_arrive);
                time_quantum = get_time_quantum(process_queue);
                next_to_arrive++;
            }
        }
    }
}
