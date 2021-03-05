#include "utils.h"

//Insert Algorithm for Sorted Linked List 
Node *insert(Node *start, Node *new) {
    if (!start || new->bt < start->bt){
        new->next = start;
        start = new;
        return start;
    }
    
    Node *prev = start;
    Node *curr = start->next;
    while (curr && curr->bt < new->bt ){
        prev = curr;
        curr = curr->next;
    }
    new->next = curr;
    prev->next = new;

    return start;
}


//Printing of Sorted Linked List
void print_ll (Node *start){
    while (start != NULL){
        printf("\nProcess Number %d: | BT: %d", start->pid, start->bt);
        start = start->next;
    }    
    
}
