#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NUM_PROCESSES 100
#define MAX_NUM_EVENTS 10000

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
    int num_processes;
    struct Node *front;
    struct Node *back;
};

enum EventType {
    PROCESS_ARRIVE = 0,
    TIME_QUANTUM_END = 1
};

struct Event {
    int time;
    enum EventType event_type;
    struct ProcessInfo *pinfo;
};

struct EventHeap
{
    struct Event *arr;
    int num_events;
    int capacity;
};


// main.c functions
int getNumProcesses();
int getTimeQuantum();
struct ProcessInfo *receiveAndProcessInput(int n);
void simulateESRR(int n, int time_quantum, struct ProcessInfo *processes);
void runNextProcess(struct EventHeap *events, struct ProcessInfo *next_process, int time_quantum, int current_time);
void printEventStatus(struct Event *event, struct ProcessQueue *queue, int is_event_handled);
void printStats(int n, struct ProcessInfo *processes);

// queue.c functions
struct ProcessQueue *createQueue();
void printQueue(struct ProcessQueue *queue);
void destroyQueue(struct ProcessQueue *queue);
void insertIntoQueue(struct ProcessQueue *queue, struct ProcessInfo *pinfo);
void insertQueueBack(struct ProcessQueue *queue, struct ProcessInfo *pinfo);
void sortQueue(struct ProcessQueue *queue);
int isQueueEmpty(struct ProcessQueue *queue);
struct ProcessInfo *popQueueFront(struct ProcessQueue *queue);

// heap.c functions
struct EventHeap *createPriorityQueue(int capacity);
void destroyPriorityQueue(struct EventHeap *priority_queue);
int isPriorityQueueEmpty(struct EventHeap *priority_queue);
void insertEvent(struct EventHeap *priority_queue, int time, enum EventType event_type, struct ProcessInfo *pinfo);
struct Event *getEarliestEvent(struct EventHeap *priority_queue);
void popEarliestEvent(struct EventHeap *priority_queue);
void heapifyUp(struct EventHeap *priority_queue, int index);
void heapifyDown(struct EventHeap *priority_queue, int index);
int compareEvents(struct Event *event1, struct Event *event2);


// helper comparator functions for qsort
int arrivalTimeComparator(const void *proc1, const void *proc2);
int pidComparator(const void *proc1, const void *proc2);