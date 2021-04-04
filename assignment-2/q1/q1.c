#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

struct wait_thread_args
{
    pid_t pid;
    char **command;
};

void *wait_command(void *args)
{
    struct wait_thread_args *thread_args = (struct wait_thread_args *)args;
    pid_t pid = thread_args->pid;
    char *command = *(thread_args->command);

    // wait for the command to finish and get its exit status
    int status;
    waitpid(pid, &status, 0);

    if (status == 0)
        printf("Command %s has completed successfully\n", command);
    else
        printf("Command %s has not completed successfully\n", command);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t wait_threads[argc - 1];

    pid_t pid = 0;

    for (int i = 1; i < argc; i++)
    {
        char *command = argv[i];
        char *command_args[] = {command, NULL};

        pid = fork();
        if (pid == 0)
        {
            // in child process
            // execute the command
            if (execv(command, command_args) == -1)
                perror("exevc failed");
        }
        else if (pid == -1)
        {
            perror("fork failed");
            return 1;
        }
        else
        {
            // in parent process
            // create a thread to wait for command to finish
            struct wait_thread_args thread_args = {pid, argv + i};
            pthread_create(wait_threads + i - 1, NULL, wait_command, (void *)&thread_args);
        }
    }

    if (pid != 0)
    {
        // wait for all wait threads (i.e. commands) to finish
        for (int i = 0; i < argc - 1; i++)
        {
            pthread_join(wait_threads[i], NULL);
        }
    }

    printf("All done, bye!\n");
    return 0;
}
