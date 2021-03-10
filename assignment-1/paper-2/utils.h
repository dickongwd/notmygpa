#include <stdio.h>
#include <stdlib.h>
#define ALTERNATIVE 1


//Structure for Sorted Linked List 
typedef struct Nodes{
   struct Nodes* next;
   unsigned int bt;
   unsigned int pid;
   unsigned int tat;
} Node;

Node *insert(Node *start, Node *new);
void print_ll (Node *start);


Node *start;
Node *process;
int num_process;
int k, t, count;
float average_tat, average_waiting_time;

