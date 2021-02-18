#include "utils.h"

struct ProcessQueue *createQueue()
{
    struct ProcessQueue *queue = (struct ProcessQueue *)malloc(sizeof(struct ProcessQueue));
    memset(queue, 0, sizeof(queue));
    return queue;
}

void insertIntoQueue(struct ProcessQueue *queue, struct ProcessInfo *pinfo)
{
    // Create node to insert
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    memset(new_node, 0, sizeof(new_node));
    new_node->pinfo = pinfo;

    // Insert node into queue in position ordered by remaining execution time
    struct Node *existing_node = queue->front;
    if (existing_node == NULL)
    {
        // Empty queue, insert to front
        queue->front = new_node;
        queue->back = new_node;
    }
    else
    {
        // Navigate to first process in queue with greater remaining execution time, if any
        while (existing_node != NULL && existing_node->pinfo->remaining_execution_time <= new_node->pinfo->remaining_execution_time)
        {
            existing_node = existing_node->next;
        }

        if (existing_node == queue->front)
        {
            // new process to be inserted in front of queue
            new_node->next = existing_node;
            existing_node->prev = new_node;
            queue->front = new_node;
        }
        else if (existing_node != NULL)
        {
            // insert new process in front of existing_node
            new_node->prev = existing_node->prev;
            existing_node->prev->next = new_node;

            new_node->next = existing_node;
            existing_node->prev = new_node;
        }
        else
        {
            // new process has longest remaining execution time, insert to back of queue
            queue->back->next = new_node;
            new_node->prev = queue->back;
            queue->back = new_node;
        }
    }
}

int isQueueEmpty(struct ProcessQueue *queue)
{
    // Returns 1 if queue is empty and 0 if queue has contents
    return queue->front == NULL;
}

struct ProcessInfo *popQueueFront(struct ProcessQueue *queue)
{
    // Check that queue has contents with isQueueEmpty first before calling this
    struct Node *front_node = queue->front;
    struct ProcessInfo *pinfo = front_node->pinfo;

    if (front_node == queue->back)
    {
        // Popping the only node in the queue
        queue->front = queue->back = NULL;
    }
    else
    {
        front_node->next->prev = NULL;
        queue->front = front_node->next;
    }

    free(front_node);
    return pinfo;
}

void destroyQueue(struct ProcessQueue *queue)
{
    while (!isQueueEmpty(queue))
    {
        free(popQueueFront(queue));
    }
    free(queue);
}

int arrivalTimeComparator(const void *proc1, const void *proc2)
{
    return ((struct ProcessInfo *)proc1)->arrival_time - ((struct ProcessInfo *)proc2)->arrival_time;
}