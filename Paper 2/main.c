#include "utils.h"


int main()
{
    printf("Enter number of processes: ");
    //10
    scanf("%d", &num_process);

    //Create Array to Store burst tiems
    Node *run = (struct Nodes*) malloc(num_process * sizeof(struct Nodes));

    //Create Nodes for the processes
    //42 68 35 1 70 25 79 59 63 65
    printf("Enter the burst times: ");
    for (int i=0; i < num_process; i++){
        process = &run[i];
        scanf("%d", &(process->bt));
        process->pid = i+1;
        start = insert(start, process);
    }
    //print_ll(start);
    
    //Algorithm to select Process
    k=2;
    t=0;
    count=1;

    //First process enters the CPU
    printf("\nProcess Order");
    printf("\n---------------------------------");
    printf("\n|%d|", start->pid);
    t += start->bt;
    start->tat = t;
    start = start->next;

    //Rest of the processes
    while (start){
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

        if (ALTERNATIVE){
            k += count % 2;
            count++;
        }
        
    }
    printf("\n---------------------------------");

    printf("\n");
    for (int i=0; i < num_process; i++){
        printf("\nProcess Number %d: | BT: %d| tat: %d", run[i].pid, run[i].bt, run[i].tat);
        average_tat += run[i].tat;
        average_waiting_time += run[i].tat - run[i].bt; 
    }

    printf("\n");
    printf("\nAverage waiting time: %.3f", average_waiting_time/num_process);
    printf("\nAverage turn around time: %.3f", average_tat/num_process);

    //Clean Up
    free(run);

    return 0;
}