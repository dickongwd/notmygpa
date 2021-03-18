#ifndef UTIL_H
#define UTIL_H

//Structure for Sorted Linked List 
typedef struct Nodes{
   struct Nodes* next;
   unsigned int bt;
   unsigned int arrival_time;
   unsigned int pid;
   unsigned int tat;
} Node;

void insert(Node *new);
void display_stats(Node *processes, int n);
void enqueue(Node *new);
void dequeue();
void print_ll (Node *start);
int move_wait_ready(int time);

extern Node *start;
extern Node *arrival_queue;

#endif

#define ALTERNATIVE 1


