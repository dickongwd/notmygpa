#include "utils.h"

/**
 * Implementing a 0-indexed heap
 * Needed for efficient implementation of event priority queue
 * to simulate when events like a process arriving or the CPU finishing
 * computing a process happens
*/

struct EventHeap *createPriorityQueue(int capacity)
{
    struct EventHeap *priority_queue = malloc(sizeof(struct EventHeap));

    priority_queue->arr = malloc(capacity * sizeof(struct Event));
    priority_queue->num_events = 0;
    priority_queue->capacity = capacity;

    return priority_queue;
}

void destroyPriorityQueue(struct EventHeap *priority_queue)
{
    free(priority_queue->arr);
    free(priority_queue);
}

int isPriorityQueueEmpty(struct EventHeap *priority_queue)
{
    return priority_queue->num_events == 0;
}

void insertEvent(struct EventHeap *priority_queue, int time, enum EventType event_type, struct ProcessInfo *pinfo)
{
    priority_queue->arr[priority_queue->num_events].time = time;
    priority_queue->arr[priority_queue->num_events].event_type = event_type;
    priority_queue->arr[priority_queue->num_events].pinfo = pinfo;

    heapifyUp(priority_queue, priority_queue->num_events);

    ++priority_queue->num_events;
}

struct Event *getEarliestEvent(struct EventHeap *priority_queue)
{
    return priority_queue->arr;
}

void popEarliestEvent(struct EventHeap *priority_queue)
{
    priority_queue->arr[0] = priority_queue->arr[priority_queue->num_events - 1];
    --priority_queue->num_events;
    heapifyDown(priority_queue, 0);
}

void heapifyUp(struct EventHeap *priority_queue, int index)
{
    int parent_index = (index - 1) / 2;
    if (compareEvents(&(priority_queue->arr[index]), &(priority_queue->arr[parent_index])) < 0)
    {
        // Swap the event up then continue heapifying up
        struct Event temp = priority_queue->arr[index];
        priority_queue->arr[index] = priority_queue->arr[parent_index];
        priority_queue->arr[parent_index] = temp;

        heapifyUp(priority_queue, parent_index);
    }
}

void heapifyDown(struct EventHeap *priority_queue, int index)
{
    int left_index = index * 2 + 1;
    int right_index = index * 2 + 2;
    int min_index;

    if (left_index >= priority_queue->num_events)
        return;
    else if (right_index >= priority_queue->num_events)
        min_index = left_index;
    else if (compareEvents(&(priority_queue->arr[left_index]), &(priority_queue->arr[right_index])) < 0)
        min_index = left_index;
    else
        min_index = right_index;

    if (compareEvents(&(priority_queue->arr[min_index]), &(priority_queue->arr[index])) < 0)
    {
        // swap event[index] for event[min_index], then heapify down from min_index
        struct Event temp = priority_queue->arr[index];
        priority_queue->arr[index] = priority_queue->arr[min_index];
        priority_queue->arr[min_index] = temp;

        heapifyDown(priority_queue, min_index);
    }
}

int compareEvents(struct Event *event1, struct Event *event2)
{
    // Sort by event time first,
    // then event type if the 2 events have the same time
    // then the process remaining execution time if the event types are both PROCESS_ARRIVE
    if (event1->time != event2->time)
        return event1->time - event2->time;

    if (event1->event_type != event2->event_type)
        return event1->event_type - event2->event_type;

    if (event1->event_type == PROCESS_ARRIVE)
        return event1->pinfo->remaining_execution_time - event2->pinfo->remaining_execution_time;
    
    return 0;
}