#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gcc source3.c && type in | a

//#define __DEBUG__

#ifdef __DEBUG__
#define DBG(...) printf(__VA_ARGS__);
#else
#define DBG(...) ;
#endif

#define MIN(a, b) (a < b) ? a : b

typedef struct
{
    int sequence;
    float arrival_time;
    float burst_time;
    float burst_time_remain;
    float waiting_time;
    float turnaround_time; // burst_time + waiting_time
} proc;

int cmpfunc(const void *a, const void *b)
{
    const proc *u1 = *(proc **)a;
    const proc *u2 = *(proc **)b;
    if (u2->burst_time_remain > u1->burst_time_remain)
        return -1;
    if (u2->burst_time_remain < u1->burst_time_remain)
        return 1;
    return 0;
}

float findMBT(proc **task_queue, int size) // Assumes Sorted
{
    float MBT;
    int mid = size / 2;
    if (size == 0)
        return 0;
    else if (size == 1)
        return task_queue[0]->burst_time_remain;

    if (size % 2 == 0) // even
        MBT = (task_queue[mid - 1]->burst_time_remain + task_queue[mid]->burst_time_remain) / 2;
    else // odd
        MBT = task_queue[mid]->burst_time_remain;

    return MBT;
}

int whosready(proc *data, proc **buffer, int size, float clock)
{
    //proc *ready_queue = (proc *)malloc(sizeof(proc) * size);
    int ready_size = 0;
    int done = 0;
    //printf("[whosready] Clock: %d\n", clock);
    for (int i = 0; i < size; i++)
    {
        //printf("[whosready] Done %d\n", done);
        if (data[i].burst_time_remain <= 0)
        {
            DBG("[whosready] PID %d done\n", data[i].sequence);
            done += 1;
        }
        else if (data[i].arrival_time <= clock)
        {
            ready_size += 1;
            buffer[ready_size - 1] = &data[i];
            DBG("[whosready] Added %d \n", data[i].sequence);
        }

        if (done >= size)
        {
            return -1;
        }
    }
    //buffer = ready_queue;
    return ready_size;
}

void run(proc **task_queue, int size, float *clock)
{

    while (1)
    {
        int end = 0;
        float time_quantum = findMBT(task_queue, size);
        DBG("[run] time_quantum %f Size %d\n", time_quantum, size);
        for (int i = 0; i < size; i++)
        {
            DBG("[run] PID %d btr %f\n", task_queue[i]->sequence, task_queue[i]->burst_time_remain);
            float time_spent = MIN(task_queue[i]->burst_time_remain, time_quantum);
            task_queue[i]->burst_time_remain -= time_spent;
            *clock += time_spent;
            if (task_queue[i]->burst_time_remain <= 0)
            {
                DBG("[run] PID %d is done\n", task_queue[i]->sequence);
                task_queue[i]->turnaround_time = *clock - task_queue[i]->arrival_time;
                task_queue[i]->waiting_time = task_queue[i]->turnaround_time - task_queue[i]->burst_time;
                end += 1;
                //break;
            }
        }
        size -= end;
        task_queue += end;
        if (size == 0)
            break;
    }
}

int start(proc *data, int size)
{
    float clock = 0;
    proc **ready_queue = (proc **)malloc(sizeof(proc *) * size);

    while (1)
    {
        DBG("[start] New queue!\n", clock);
        memset(ready_queue, 0, sizeof(proc *) * size);
        int ready_size = whosready(data, ready_queue, size, clock);

        if (ready_size == -1)
        { // done
            DBG("[start] Done at %f\n", clock);
            return 1;
        }
        else if (ready_size == 0)
        {
            DBG("[start] Queue empty at time %f\n", clock);
            clock += 1;
            //continue;
        }
        else if (ready_size == 1) // no split
        {
            DBG("[start] No split at time %f\n", clock);
            run(ready_queue, 1, &clock);
        }
        else // split and run
        {
            DBG("[start] Split and run %f\n", clock);
            qsort(ready_queue, ready_size, sizeof(proc *), cmpfunc);
            float MBT = findMBT(ready_queue, ready_size);
            int lsize;
            for (lsize = 0; lsize < ready_size; lsize++)
            {
                if (ready_queue[lsize]->burst_time_remain > MBT)
                {
                    lsize = lsize;
                    break;
                }
            }
            int hsize = ready_size - lsize;

            DBG("[start] About to run light q: %d %d\n", lsize, ready_size);
            run(ready_queue, lsize, &clock);
            DBG("[start] About to run heavy q: %d %d\n", hsize, ready_size);
            run(ready_queue + lsize, hsize, &clock);
        }
        //sleep(1);
    }

    free(ready_queue);
}

void display(proc *data, int size)
{
    printf("%4s\t %10s\t %10s\n", "Proc", "Turnaround", "Waiting");
    for (int i = 0; i < size; i++)
    {
        printf("P%-4d\t %10.1f\t %10.1f\n", data[i].sequence, data[i].turnaround_time, data[i].waiting_time);
    }

    float avg_turnaround_time = 0;
    float avg_waiting_time = 0;
    for (int i = 0; i < size; i++)
    {
        avg_turnaround_time += data[i].turnaround_time;
        avg_waiting_time += data[i].waiting_time;
    }
    avg_turnaround_time = avg_turnaround_time / size;
    avg_waiting_time = avg_waiting_time / size;
    printf("Average\t %10.1f\t %10.1f\n", avg_turnaround_time, avg_waiting_time);
}

int getinput(proc **buffer)
{
    int size;
    printf("Number of process\n");
    scanf("%d", &size);
    proc *data = (proc *)malloc(sizeof(proc) * size);
    printf("Time for each job in sequence in ms\n");

    for (int i = 0; i < size; i++)
    {
        printf("Enter Burst Time for process %d:\n", i + 1);
        data[i].sequence = i + 1;
        scanf("%f", &data[i].burst_time);
        printf("\n");
        data[i].burst_time_remain = data[i].burst_time;
        data[i].turnaround_time = 0;
        data[i].waiting_time = 0;
    }
    for (int i = 0; i < size; i++)
    {
        printf("Enter Arrival Time for process %d:\n", i + 1);
        scanf("%f", &data[i].arrival_time);
        printf("\n");
    }

    *buffer = data;
    return size;
}

int main()
{
    proc *data;
    int size = getinput(&data);
    start(data, size);
    display(data, size);
    free(data);
}