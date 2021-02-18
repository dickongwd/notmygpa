#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

const int MAX_NUM_PROCESS = 10;

void handleProcess(Process *p, int *ct, int *npd)
{
    int tempct = *ct;
    int tempnpd = *npd;

    // If BT <= TQ
    if (p->btleft <= p->tq)
    {
        tempct += p->btleft;
        completeProcess(p, tempct);
        ++tempnpd;
    }
    // Else if (BT > TQ) && BT <= (TQ + 30%*TQ) && P == HIGH
    else if (p->btleft > p->tq &&
             p->btleft <= p->tq + (p->tq * 3 / 10) &&
             p->p == 3)
    {
        tempct += p->btleft;
        completeProcess(p, tempct);
        ++tempnpd;
    }
    // Else if (BT > TQ) && BT <= (TQ + 20%*TQ) && P == MEDIUM OR LOW
    else if (p->btleft > p->tq &&
             p->btleft <= p->tq + (p->tq / 5) &&
             (p->p == 1 || p->p == 2))
    {
        tempct += p->btleft;
        completeProcess(p, tempct);
        ++tempnpd;
    }
    else
    {
        tempct += p->tq;
        p->btleft -= p->tq;
    }

    // Write to output
    *ct = tempct;
    *npd = tempnpd;
}

void completeProcess(Process *p, int ct)
{
    p->btleft = 0;
    p->done = true;
    p->tat = ct - p->at;
    p->wt = p->tat - p->bt;
}

void appendTask(Task **head, int p, int st, int et)
{
    Task *newTask = malloc(sizeof(Task));
    newTask->id = p;
    newTask->st = st;
    newTask->et = et;
    newTask->next = NULL;

    if (*head == NULL)
    {
        *head = newTask;
    }
    else
    {
        Task *end = *head;
        while (end->next != NULL)
        {
            end = end->next;
        }
        end->next = newTask;
    }
}

void displayStats(const int n, const Process p[])
{
    int totalwt = 0, totaltat = 0;
    printf("+-----+------------+--------------+-----------------+\n");
    printf("| No. | Burst Time | Waiting Time | Turnaround Time |\n");
    printf("+-----+------------+--------------+-----------------+\n");
    for (int i = 0; i < n; ++i)
    {
        printf("| %3d | %10d | %12d | %15d |\n",
               i + 1,
               p[i].bt,
               p[i].wt,
               p[i].tat);
        totalwt += p[i].wt;
        totaltat += p[i].tat;
    }
    printf("+-----+------------+--------------+-----------------+\n");
    printf("\n");

    printf("Average Waiting Time: %.2f\n", (double)totalwt / n);
    printf("Average Turnaround Time: %.2f\n", (double)totaltat / n);
    printf("\n");
}

void displayGanttChart(Task *head)
{
    Task *current = head;
    int delta = 0;
    int numTasks = 0;
    int numBlocks = 0;

    if (head->st != 0)
        ++numBlocks;
    while (current != NULL)
    {
        delta = gcd(delta, current->st);
        delta = gcd(delta, current->et);
        if (current->next != NULL && current->next->st != current->et)
            ++numBlocks;
        ++numBlocks;
        ++numTasks;
        current = current->next;
    }

    int processId[numTasks];
    int startTime[numTasks];
    int endTime[numTasks];

    int blockId[numBlocks];
    int blockEnd[numBlocks];
    int steps[numBlocks];

    current = head;
    int currentTime = 0;
    for (int i = 0; i < numTasks; ++i)
    {
        processId[i] = current->id + 1;
        startTime[i] = current->st;
        endTime[i] = current->et;
        currentTime = current->et;
        current = current->next;
    }

    int blockNum = 0;
    currentTime = 0;
    for (int i = 0; i < numTasks; ++i)
    {
        if ((i == 0 && startTime[i] == 0) ||
            (i != 0 && startTime[i] == endTime[i - 1]))
        {
            blockId[blockNum] = processId[i];
            blockEnd[blockNum] = endTime[i];
            steps[blockNum] = (endTime[i] - currentTime) / delta;
            if (steps[blockNum] < 6)
                steps[blockNum] = 6;
            currentTime = endTime[i];
            ++blockNum;
        }
        else
        {
            // Idle block
            blockId[blockNum] = -1;
            blockEnd[blockNum] = startTime[i];
            steps[blockNum] = (startTime[i] - currentTime) / delta;
            if (steps[blockNum] < 6)
                steps[blockNum] = 6;
            currentTime = startTime[i];
            ++blockNum;

            blockId[blockNum] = processId[i];
            blockEnd[blockNum] = endTime[i];
            steps[blockNum] = (endTime[i] - currentTime) / delta;
            if (steps[blockNum] < 6)
                steps[blockNum] = 6;
            currentTime = endTime[i];
            ++blockNum;
        }
    }

    // Top bar
    printf("+");
    for (int i = 0; i < numBlocks; ++i)
    {
        for (int j = 0; j < steps[i]; ++j)
        {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    // Print process no.
    printf("|");
    for (int i = 0; i < numBlocks; ++i)
    {
        if (blockId[i] == -1)
            printf(" idle");
        else
            printf(" P%-3d", blockId[i]);
        for (int j = 0; j < steps[i] - 5; ++j)
        {
            printf(" ");
        }
        printf("|");
    }
    printf("\n");

    // Bottom bar
    printf("+");
    for (int i = 0; i < numBlocks; ++i)
    {
        for (int j = 0; j < steps[i]; ++j)
        {
            printf("-");
        }
        printf("+");
    }
    printf("\n");

    // Print time
    printf("0");
    for (int i = 0; i < numBlocks; ++i)
    {
        char temp[10];
        sprintf(temp, "%d", blockEnd[i]);
        for (int j = 0; j < steps[i] - strlen(temp) + 1; ++j)
        {
            printf(" ");
        }
        printf("%d", blockEnd[i]);
    }
    printf("\n\n");

    // Get number of context switches
    int numContextSwitch = 0;
    for (int i = 1; i < numBlocks; ++i)
    {
        if (blockId[i] != blockId[i - 1])
            ++numContextSwitch;
    }
    printf("Number of context switches: %d\n", numContextSwitch);
}

int getNumProcess()
{
    int n;

    do
    {
        // Get number of processes
        printf("Enter number of processes:\n");
        scanf("%d", &n);
        printf("\n");

        // Handling invalid input
        if (n > MAX_NUM_PROCESS)
        {
            printf(
                "Number of processes exceeds limit of %d.\n",
                MAX_NUM_PROCESS);
        }
        if (n <= 0)
        {
            printf("That is not a valid number of processes (1-%d).\n",
                   MAX_NUM_PROCESS);
        }
    } while (n > MAX_NUM_PROCESS || n <= 0);

    return n;
}

int getTimeQuantum()
{
    int tq;

    do
    {
        // Get time quantum
        printf("Enter time quantum:\n");
        scanf("%d", &tq);
        printf("\n");

        // Handling invalid input
        if (tq <= 0)
            printf("Time quantum entered is not valid.\n");
    } while (tq <= 0);
}

void getBurstTime(int n, Process p[])
{
    bool valid;

    do
    {
        valid = true;

        // Get burst times of all processes
        printf("Enter burst times of all processes (separated by a space):\n");
        for (int i = 0; i < n; ++i)
        {
            scanf("%d", &p[i].bt);
            if (p[i].bt <= 0)
                valid = false;
            p[i].btleft = p[i].bt;
        }
        printf("\n");

        // Handling invalid input
        if (!valid)
            printf("At least one of the burst times entered is not valid.\n");

    } while (!valid);
}

void getArrivalTime(int n, Process p[])
{
    bool valid;

    do
    {
        valid = true;

        // Get arrival times of all processes
        printf("Enter arrival times for all processes (separated by a space):\n");
        for (int i = 0; i < n; ++i)
        {
            scanf("%d", &p[i].at);
            if (p[i].at < 0)
                valid = false;
        }
        printf("\n");

        if (!valid)
            printf("At least one of the arrival times entered is not valid.\n");

    } while (!valid);
}

void getPriority(int n, Process p[])
{
    bool valid;

    do
    {
        valid = true;

        // Get priority for all processes
        printf("Enter priority for all processes (separated by a space):\n");
        for (int i = 0; i < n; ++i)
        {
            scanf("%d", &p[i].p);
            if (p[i].p != 1 && p[i].p != 2 && p[i].p != 3)
                valid = false;
        }
        printf("\n");

        if (!valid)
            printf("At least one of the priorities entered is not valid.\n");

    } while (!valid);
}

void setTimeQuantum(int n, Process p[], int tq)
{
    /*
     * Low priority processes get TQ - 20%TQ
     * Medium priority processes get TQ
     * High priority processes get TQ + 20%TQ
     */
    for (int i = 0; i < n; ++i)
    {
        p[i].tq = tq;
        // Time quantums are truncated to integers for simplicity
        if (p[i].p == 1)
            p[i].tq -= tq / 5;
        if (p[i].p == 3)
            p[i].tq += tq / 5;
    }
}

int gcd(int a, int b)
{
    return b == 0 ? a : gcd(b, a % b);
}
