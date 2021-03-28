#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUM_PROCESS 8
#define NUM_RESOURCE 3

pthread_rwlock_t global_rwlocks[NUM_RESOURCE];
pthread_t global_threads[NUM_PROCESS];

void exit_program();
void simulate_deadlock();
void simulate_starvation();
void simulate_race();


void generate_random_state(int available[NUM_RESOURCE], int allocation[NUM_PROCESS][NUM_RESOURCE], int request[NUM_PROCESS][NUM_RESOURCE]);
int is_unsafe(int available[NUM_RESOURCE], int allocation[NUM_PROCESS][NUM_RESOURCE], int request[NUM_PROCESS][NUM_RESOURCE], int finish[NUM_PROCESS]);

void initialize_rwlocks();
void simulate_rw_processes();
void *reader_thread(void *arg);
void *writer_thread(void *arg);

void read_file_number(char *file_name);
void increment_file(char *file_name);
void initialize_file(char *file_name);
void fork_with_handler(int *is_parent);
void handle_fork_error();

int main(int argc, char *argv[]) {
    if (argc != 2) 
        exit_program();

    if (strcmp(argv[1], "deadlock") == 0)
        simulate_deadlock();
    else if (strcmp(argv[1], "starvation") == 0)
        simulate_starvation();
    else if (strcmp(argv[1], "race") == 0)
        simulate_race();
    else
        exit_program();
}


void exit_program() {
    printf("Usage: ./q4 {deadlock,starvation,race}\n");
    exit(0);
}


void simulate_deadlock() {
    printf("Generating a random deadlock scenario with %d processes and %d resources...\n\n", NUM_PROCESS, NUM_RESOURCE);
    
    int available[NUM_RESOURCE];
    int allocation[NUM_PROCESS][NUM_RESOURCE];
    int request[NUM_PROCESS][NUM_RESOURCE];
    int finish[NUM_PROCESS];

    srand(time(0));

    do {
        generate_random_state(available, allocation, request);
    } 
    while (!is_unsafe(available, allocation, request, finish));

    for(int i=0;i<NUM_PROCESS;i++) {
        printf("Process %d is requesting the following resources: ", i);
        for(int j=0;j<NUM_RESOURCE;j++) if (request[i][j]) printf("%d ", j);
        printf("\n");

        printf("Process %d is holding onto the following resources: ", i);
        for(int j=0;j<NUM_RESOURCE;j++) if (allocation[i][j]) printf("%d ", j);
        printf("\n\n");
    }

    printf("Processes ");
    for(int i=0;i<NUM_PROCESS;i++) if (!finish[i]) printf("%d ", i);
    printf("are in deadlock\n");
}

void generate_random_state(int available[NUM_RESOURCE], int allocation[NUM_PROCESS][NUM_RESOURCE], int request[NUM_PROCESS][NUM_RESOURCE]) {

    memset(available, 0, sizeof(int) * NUM_RESOURCE);
    memset(request, 0, sizeof(int) * NUM_PROCESS * NUM_RESOURCE);
    memset(allocation, 0, sizeof(int) * NUM_PROCESS * NUM_RESOURCE);
    
    for(int j=0;j<NUM_RESOURCE;j++) {
        allocation[rand() % NUM_PROCESS][j] = 1;

        for (int i=0;i<NUM_PROCESS;i++) if (!allocation[i][j]) {
            request[i][j] = rand() % 2;
        }
    }

}

int is_unsafe(int available[NUM_RESOURCE], int allocation[NUM_PROCESS][NUM_RESOURCE], int request[NUM_PROCESS][NUM_RESOURCE], int finish[NUM_PROCESS]) {
    /*
    Using Banker's unsafe state detection algorithm to check if state is unsafe
    This function returns the deadlocked processes in the argument finish
    */

    int work[NUM_RESOURCE];
    memset(finish, 0, sizeof(int) * NUM_PROCESS);
    memcpy(work, available, sizeof(int) * NUM_RESOURCE);

    int change_occured = 1;
    while (change_occured) {
        change_occured = 0;

        for(int i=0;i<NUM_PROCESS;i++) {
            if (finish[i])
                continue;
            
            int fufillable = 1;
            for(int j=0;j<NUM_RESOURCE;j++) {
                if (request[i][j] > work[j]) {
                    fufillable = 0;
                    break;
                }
            }
            if (!fufillable) continue;
            
            // Process i can be fufilled with the current available resources
            for (int j=0;j<NUM_RESOURCE;j++)
                work[j] += allocation[i][j];
            finish[i] = 1;
            change_occured = 1;
        }
    }

    for(int i=0;i<NUM_PROCESS;i++)
        if (!finish[i]) return 1;

    return 0;
}

void simulate_starvation() {
    /*
    Simulates writer starvation on a read-write lock

    Unix doesn't have builtin support for interprocess semaphores,
    so we will be threads to simulate it to reduce the code size
    */

    initialize_rwlocks();
    simulate_rw_processes();
}

void initialize_rwlocks() {
    for(int j=0;j<NUM_RESOURCE;j++)
        pthread_rwlock_init(&global_rwlocks[j], PTHREAD_RWLOCK_PREFER_READER_NP);
}

void simulate_rw_processes() {
    pthread_create(&global_threads[0], NULL, writer_thread, NULL);
    
    for(int i=1;i<NUM_PROCESS;i++) {
        pthread_create(&global_threads[i], NULL, reader_thread, (void *)i);
        sleep(1);
    }

    // Wait for all threads to terminate before closing
    for(int i=1;i<NUM_PROCESS;i++) {
        pthread_join(global_threads[i], NULL);
    }
}

void *reader_thread(void *arg) {
    int id = (int)arg;

    printf("Reader process %d created!\n", id);

    for(int i=0;i<10;i++) {
        for(int j=0;j<NUM_RESOURCE;j++) {
            pthread_rwlock_rdlock(&global_rwlocks[j]);
            printf("Reader process %d acquiring lock %d for reading\n", id, j);
        }
        
        sleep(1 + (rand() % 2)); // Sleep between 1 to 2 seconds to simulate reading

        for(int j=0;j<NUM_RESOURCE;j++) {
            pthread_rwlock_unlock(&global_rwlocks[j]);
            printf("Reader process %d released lock %d\n", id, j);
        }
    }
}

void *writer_thread(void *arg) {
    printf("Writer process created!\n");

    for(int i=0;i<10;i++) {
        for(int j=0;j<NUM_RESOURCE;j++) {
            pthread_rwlock_wrlock(&global_rwlocks[j]);
            printf("Writer process acquiring lock %d for reading\n", j);
        }


        sleep(1 + (rand() % 2)); // Sleep between 1 to 2 seconds to simulate reading


        for(int j=0;j<NUM_RESOURCE;j++) {
            pthread_rwlock_unlock(&global_rwlocks[j]);
            printf("Writer process released lock %d\n", j);
        }

    }
}

void simulate_race() {
    /* 
    Illustrates a race condition with 8 processes (non-atomically) incrementing 
    a single number in 3 files 100 times
    */

    printf("Simulating race condition with 8 processes and 3 files...\n\n");
    printf("We will increment to the number in res-1.txt, res-2.txt, and res-3.txt 100 times each,\n");
    printf("Meaning the final value in each of them should be 800\n\n");
    

    // initialize the values of the 3 resource files to 0 
    initialize_file("res-1.txt");
    initialize_file("res-2.txt");
    initialize_file("res-3.txt");
    
    // Generate 8 processes
    pid_t wpid;
    int is_parent = 1, curnum;
    fork_with_handler(&is_parent);
    fork_with_handler(&is_parent);
    fork_with_handler(&is_parent);

    for(int i=0;i<100;i++) {
        increment_file("res-1.txt");
        increment_file("res-2.txt");
        increment_file("res-3.txt");
    }

    if (is_parent) {
        while ((wpid = wait(0)) > 0); // Wait for all children to terminate
        read_file_number("res-1.txt");
        read_file_number("res-2.txt");
        read_file_number("res-3.txt");
    }
}

void read_file_number(char *file_name) {
    int curnum;
    FILE *fd;

    fd = fopen(file_name, "r");
    fscanf(fd, "%d", &curnum);
    fclose(fd);
    printf("%s has the final value %d\n", file_name, curnum);
}

void increment_file(char *file_name) {
    FILE *fd;
    int curnum;
    fd = fopen(file_name, "r");
    fscanf(fd, "%d", &curnum);
    fclose(fd);

    fd = fopen(file_name, "w");
    fprintf(fd, "%d", curnum+1);
    fclose(fd);
}

void initialize_file(char *file_name) {
    FILE *fd = fopen(file_name, "w");
    fprintf(fd, "%d", 0);
    fclose(fd);
}

void fork_with_handler(int *is_parent) {
    pid_t pid = fork();
    if (pid < 0) handle_fork_error();
    *is_parent &= (pid != 0);
}


void handle_fork_error() {
    printf("Something went wrong while creating the child process\n");
    exit(0);
}