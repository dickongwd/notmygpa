/*
 * Paper 5: Improved Round Robin CPU Scheduling Algorithm
 * 
 * Improved Round Robin algorithm
 *      reduces the number of context switches,
 *      gives fair time slices of the CPU to all processes,
 *      avoids starvation of processes and
 *      reduces the time a process spends in the waiting state.
 * 
 * The algorithm combines FCFS, SJF and RR algorithms into one.
 * 
 * Priority and time quantum
 *      All processes in the queue is issued a priority (low, medium, high).
 *      Processes with lower priority have smaller time slices than those with
 *      higher priority.
 * 
 * Reducing number of context swithces
 *      If the execution time that remains after a time slie is very little,
 *      then the process will finish its execution in the next time slice.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "util.h"

int main(void)
{
    int numProcesses = getNumProcess(),
        timeQuantum = getTimeQuantum(),
        currentTime = 0,
        numProcessesDone = 0,
        numCurrentProcess = 0,
        numTasks = 0;

    Process processes[MAX_NUM_PROCESS];
    memset(processes, 0, sizeof(processes));

    Task *head = NULL;

    // Getting input
    getBurstTime(numProcesses, processes);
    getArrivalTime(numProcesses, processes);
    getPriority(numProcesses, processes);

    // Set time quantum for each individual process
    setTimeQuantum(numProcesses, processes, timeQuantum);

    // Repeat algorithm until all processes are done
    while (numProcessesDone != numProcesses)
    {
        bool idle = true;

        // Check for processes with very low burst time
        // Define low burst time as <= TQ/5
        for (int i = 0; i < numProcesses; ++i)
        {
            Process *currentProcess = &processes[i];

            // If process is done or has not arrived, skip
            if (currentProcess->done || currentProcess->at > currentTime)
                continue;

            // If process has very low burst time, execute it
            if (currentProcess->btleft <= currentProcess->tq / 5)
            {
                int startTime = currentTime;
                handleProcess(currentProcess, &currentTime, &numProcessesDone);
                appendTask(&head, i, startTime, currentTime);
                idle = false;
            }
        }

        // Round robin for each process
        for (int i = 0; i < numProcesses; ++i)
        {
            Process *currentProcess = &processes[i];

            // If process is not done and has arrived
            if (!currentProcess->done && currentProcess->at <= currentTime)
            {
                int startTime = currentTime;
                handleProcess(currentProcess, &currentTime, &numProcessesDone);
                appendTask(&head, i, startTime, currentTime);
                idle = false;
            }
        }

        // If CPU did not execute any process
        if (idle)
        {
            int minArrivalTime = INT_MAX;

            // Find next undone task and wait until it arrives
            for (int i = 0; i < numProcesses; ++i)
            {
                if (!processes[i].done && processes[i].at < minArrivalTime)
                    minArrivalTime = processes[i].at;
            }
            currentTime = minArrivalTime;
        }
    }

    // Print results
    displayStats(numProcesses, processes);
    displayGanttChart(head);

    return 0;
}
