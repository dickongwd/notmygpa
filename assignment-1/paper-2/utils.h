#ifndef UTIL_H
#define UTIL_H

//Structure for Sorted Linked List 
typedef struct Nodes{
   struct Nodes* next;
   unsigned int bt;
   unsigned int pid;
   unsigned int tat;
} Node;

Node *insert(Node *start, Node *new);
void display_stats(Node *processes, int n);

#endif

