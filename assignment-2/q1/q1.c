/*
Write a C program, q1.c,to takes a list of command line arguments, each of which is the full
path of a command (such as /bin/ls, /bin/ps, /bin/date, /bin/who, /bin/uname etc). Assume
the number of such commands is N. Your program would create N direct child processes. The
parent of these child processes is your program. Each child executing one of the N commands.
You should make sure that these N commands are executed concurrently, not sequentially one
after the other. The parent process should be waiting for each child process to terminate. When
a child process terminates, the parent process should print one line on the standard output
stating that the relevant command has completed successfully or not successfully (such as
"Command /bin/who has completed successfully", or "Command /bin/who has not completed
successfully"). Once all the children processes have terminated, the parent process should print
"All done, bye!" before it itself terminates. Note: Do not use function system in this question.
You can use the different types of execl function.
*/
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
            if (execv(command, command_args) == -1)
                perror("exevc");
        }
        else if (pid == -1)
        {
            perror("fork");
            return 1;
        }
        else
        {
            struct wait_thread_args thread_args = {pid, argv + i};
            pthread_create(wait_threads + i - 1, NULL, wait_command, (void *)&thread_args);
        }
    }

    if (pid != 0)
    {
        for (int i = 0; i < argc - 1; i++)
        {
            pthread_join(wait_threads[i], NULL);
        }
    }

    printf("All done, bye!\n");
    return 0;
}
