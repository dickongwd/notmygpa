#include <stdio.h>
#include "utils.h"


//Insert Algorithm for Sorted Linked List 
Node *insert(Node *start, Node *new) {
    if (!start || new->bt < start->bt){ 
        new->next = start;
        start = new;
        return start;
    }
    
    Node *prev = start;
    Node *curr = start->next;
    while (curr && curr->bt < new->bt ){
        prev = curr;
        curr = curr->next;
    }
    new->next = curr;
    prev->next = new;

    return start;
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
