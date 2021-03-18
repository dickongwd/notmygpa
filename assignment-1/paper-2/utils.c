#include <stdio.h>
#include "utils.h"


//Insert Algorithm for Sorted Linked List 
void insert(Node *new) {
    if (!start || new->bt < start->bt){ 
        new->next = start;
        start = new;
    } else {
        Node *prev = start;
        Node *curr = start->next;
        while (curr && curr->bt <= new->bt ){
            prev = curr;
            curr = curr->next;
        }
        new->next = curr;
        prev->next = new;
    }
}

void enqueue(Node *new){
    if (!arrival_queue || new->arrival_time < arrival_queue->arrival_time){ 
        new->next = arrival_queue;
        arrival_queue = new;
    } else{
        Node *prev = arrival_queue;
        Node *curr = arrival_queue->next;
        while (curr && curr->arrival_time <= new->arrival_time ){
            prev = curr;
            curr = curr->next;
        }
        new->next = curr;
        prev->next = new;
    }
}




void dequeue(){
    arrival_queue = arrival_queue->next;
}

int move_wait_ready(int time){
    Node * process;

    if (!start && time < arrival_queue->arrival_time) {
        time = arrival_queue->arrival_time;
    } 


    while (arrival_queue && arrival_queue->arrival_time <= time){
        Node *process = arrival_queue;
        dequeue();
        process->next = NULL;
        insert(process);
    }
    return time;
}




void display_stats(Node *processes, int n)
{
    int total_waiting_time = 0, total_turnaround_time = 0;
    printf("+-----+------------+--------------+-----------------+\n");
    printf("| PID | Burst Time | Waiting Time | Turnaround Time |\n");
    printf("+-----+------------+--------------+-----------------+\n");
    for (int i = 0; i < n; i++)
    {
        printf("| %3d | %10d | %12d | %15d |\n",
               processes[i].pid,
               processes[i].bt,
               processes[i].tat - processes[i].bt,
               processes[i].tat);
        total_waiting_time += processes[i].tat - processes[i].bt;
        total_turnaround_time += processes[i].tat;
    }
    printf("+-----+------------+--------------+-----------------+\n");
    printf("\n");

    printf("Average Waiting Time: %.2f\n", (double)total_waiting_time / n);
    printf("Average Turnaround Time: %.2f\n", (double)total_turnaround_time / n);
    printf("\n");
}


void print_ll (Node *first){
    while (first != NULL){
        printf("\nProcess Number %d: | BT: %d| Arrival Time: %d", first->pid, first->bt, first->arrival_time);
        first = first->next;
    }    
}

