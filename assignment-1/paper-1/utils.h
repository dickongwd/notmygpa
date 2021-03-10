#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NUM_PROCESSES 10

struct ProcessInfo
{
    int pid;
    int arrival_time;
    int remaining_execution_time;
    int initial_execution_time;
    int turn_around_time;
};

struct Node
{
    struct ProcessInfo *pinfo;
    struct Node *next;
    struct Node *prev;
};

struct ProcessQueue
{
    struct Node *front;
    struct Node *back;
};

// main.c functions
int getNumProcesses();
int getTimeQuantum();
struct ProcessInfo *receiveAndProcessInput(int n);
void simulateESRR(int n, int time_quantum, struct ProcessInfo *processes);
void runExistingProcesses(struct ProcessQueue *queue, int *nextCpuIdleTime, int runUntil, int time_quantum);
void printStats(int n, struct ProcessInfo *processes);

// utils.c functions
struct ProcessQueue *createQueue();
void printQueue(struct ProcessQueue *queue);
void destroyQueue(struct ProcessQueue *queue);
void insertIntoQueue(struct ProcessQueue *queue, struct ProcessInfo *pinfo);
void insertQueueBack(struct ProcessQueue *queue, struct ProcessInfo *pinfo);
int isQueueEmpty(struct ProcessQueue *queue);
struct ProcessInfo *popQueueFront(struct ProcessQueue *queue);

// helper comparator functions for qsort
int arrivalTimeComparator(const void *proc1, const void *proc2);
int pidComparator(const void *proc1, const void *proc2);