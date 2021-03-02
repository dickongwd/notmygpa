#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int turnaround_time;
} process;

typedef struct node
{
    process *proc;
    struct node *next;
} node;

typedef struct
{
    node *front;
} queue;

int compare_arrival_time(const void *proc_a, const void *proc_b);
void destroy_processes(process *processes, int n);

queue *init_queue();
int is_empty(queue *process_queue);
int get_queue_length(queue *process_queue);
void enqueue(queue *process_queue, process *proc);
void enqueue_end(queue *process_queue, process *proc);
process *dequeue(queue *process_queue);
process *peek_front(queue *process_queue);
void move_front_to_end(queue *process_queue);
void destroy_queue(queue *process_queue);

void display_stats(process *processes, int n);

int get_num_processes();
process *get_processes_info(int n);
int get_max_burst_time(queue *process_queue);
int get_time_quantum(queue *process_queue);
void do_process(queue *process_queue, int *cpu_time, int *time_quantum);
void simulate_edrr(queue *process_queue, process *processes, int n);
