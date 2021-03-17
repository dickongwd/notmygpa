#include "utils.h"

struct ProcessQueue *createQueue()
{
    struct ProcessQueue *queue = (struct ProcessQueue *)malloc(sizeof(struct ProcessQueue));
    queue->num_processes = 0;
    queue->front = NULL;
    queue->back = NULL;
    return queue;
}

/*
void insertIntoQueue(struct ProcessQueue *queue, struct ProcessInfo *pinfo)
{
    // Create node to insert
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->pinfo = pinfo;
    new_node->next = NULL;
    new_node->prev = NULL;

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
*/

void insertQueueBack(struct ProcessQueue *queue, struct ProcessInfo *pinfo)
{
    // Create node to insert
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    new_node->pinfo = pinfo;
    new_node->next = NULL;
    new_node->prev = NULL;

    // Insert node into queue in position ordered by remaining execution time
    if (queue->num_processes == 0)
    {
        // Empty queue, insert to back
        queue->front = new_node;
        queue->back = new_node;
    }
    else
    {
        queue->back->next = new_node;
        new_node->prev = queue->back;
        queue->back = new_node;
    }
    ++queue->num_processes;
}

void sortQueue(struct ProcessQueue *queue)
{
    // O(n^2) bubble sort
    if (queue->num_processes < 2)
        return;

    int unsorted = 1;
    struct Node *cur_node, *next_node;
    while (unsorted)
    {
        cur_node = queue->front;
        unsorted = 0;

        while (cur_node != queue->back)
        {
            next_node = cur_node->next;
            if (cur_node->pinfo->remaining_execution_time > next_node->pinfo->remaining_execution_time)
            {
                unsorted = 1;
                // swap processes to preserve increasing order of node
                struct ProcessInfo *temp = cur_node->pinfo;
                cur_node->pinfo = next_node->pinfo;
                next_node->pinfo = temp;
            }
            cur_node = next_node;
        }
    }
}

int isQueueEmpty(struct ProcessQueue *queue)
{
    return queue->num_processes == 0;
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
    --queue->num_processes;
    return pinfo;
}

void printQueue(struct ProcessQueue *queue)
{
    struct Node *cur = queue->front;

    while (cur != NULL)
    {
        printf("%d ", cur->pinfo->pid);
        cur = cur->next;
    }
    printf("\n");
}

void destroyQueue(struct ProcessQueue *queue)
{
    while (!isQueueEmpty(queue))
    {
        free(popQueueFront(queue));
    }
    free(queue);
}
