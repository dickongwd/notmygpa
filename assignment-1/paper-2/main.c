#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


Node *start=NULL;
Node *arrival_queue=NULL;


int main()
{

    Node *process;
    int num_process;
    int k, t=0, count, process_left;


    printf("Enter number of processes: ");
    //10
    scanf("%d", &num_process);
    process_left = num_process;

    //Create Array to Store burst tiems
    Node *run = (struct Nodes*) malloc(num_process * sizeof(struct Nodes));

    //Create Nodes for the processes
    //42 68 35 1 70 25 79 59 63 65
    printf("Enter the burst times: ");
    for (int i=0; i < num_process; i++){
        scanf("%d", &(run[i].bt));
        run[i].pid = i+1;
        run[i].next = NULL;
        //start = insert(start, process);
    }

    //0 0 0 0 0 0 0 0 0 0
    printf("Enter Arrival times: ");
    for (int i=0; i < num_process; i++){
        scanf("%d", &(run[i].arrival_time));
        enqueue(&run[i]);
    }
    


    //Algorithm to select Process
    k=2;
    t=0;
    count=1;

    //First process enters the CPU
    printf("\nProcess Order");
    printf("\n---------------------------------");
    t = move_wait_ready(t);

    printf("\n|%d|", start->pid);
    t += start->bt;
    start->tat = t;
    start = start->next;
    process_left--;

    //Rest of the processes
    while (process_left){
        t = move_wait_ready(t);
        if (!(start->next)){
            //last process
            printf("%d|", start->pid);
            t += start->bt;
            start->tat = t;
            start = start->next;
        } else if (k * start->bt > t + (start->next->bt)){
            printf("%d|", start->next->pid);
            t += start->next->bt;
            start->next->tat = t;
            start->next = start->next->next;
        } else {
            printf("%d|", start->pid);
            t += start->bt;
            start->tat = t;
            start = start->next;
        }
        process_left--;
        if (ALTERNATIVE){
            k += count % 2;
            count++;
        }
        
    }
    printf("\n---------------------------------\n");


    display_stats(run, num_process);

    //Clean Up
    free(run);

    return 0;
}