struct ProcessInfo {
    int pid;
    int remaining_execution_time;
    int initial_execution_time;
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
struct ProcessInfo *createProcess(int pid, int initial_execution_time);
void insertIntoQueue(struct ProcessQueue *queue, struct ProcessInfo *pinfo);
int isQueueEmpty(struct ProcessQueue *queue);
struct ProcessInfo *popQueueFront(struct ProcessQueue *queue);