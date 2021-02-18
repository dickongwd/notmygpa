#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NUM_PROCESSES 10

struct ProcessInfo {
    int pid;
    int arrival_time;
    int remaining_execution_time;
    int initial_execution_time;
    int turn_around_time;
};

struct Node {
   struct ProcessInfo *pinfo; 
   struct Node *next;
   struct Node *prev;
};

struct ProcessQueue {
    struct Node *front;
    struct Node *back;
};

struct ProcessQueue *createQueue();
void destroyQueue(struct ProcessQueue *queue);
void insertIntoQueue(struct ProcessQueue *queue, struct ProcessInfo *pinfo);
int isQueueEmpty(struct ProcessQueue *queue);
struct ProcessInfo *popQueueFront(struct ProcessQueue *queue);

int arrivalTimeComparator(const void *proc1, const void *proc2);