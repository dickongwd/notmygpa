#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "util.h"

#define LINE_BUFSIZE 1024
#define LINE_NUM_ARGS 64
#define LINE_DELIMITERS " \t\n"

char *sh_read_line();
char **sh_parse_line(char *line);
FILE *sh_get_new_stdout(char **args);
void sh_execute(char **args, FILE *new_stdout);
void sh_exit();

int main()
{
    char *line;
    char **args;
    FILE *new_stdout = NULL;

    do
    {
        line = sh_read_line();
        args = sh_parse_line(line);
        new_stdout = sh_get_new_stdout(args);
        sh_execute(args, new_stdout);
    } while (1);
    return EXIT_SUCCESS;
}

char *sh_read_line()
{
    int bufsize = LINE_BUFSIZE;
    char *buffer = (char *)malloc_or_exit(bufsize * sizeof(char));

    printf("q3sh $ ");
    if (getline(&buffer, (size_t *)&bufsize, stdin) == -1)
    {
        // File reached EOF, no more commands
        if (feof(stdin))
            exit(EXIT_SUCCESS);
        else
        {
            fprintf(stderr, "getline failure\n");
            exit(EXIT_FAILURE);
        }
    }

    trim(buffer);
    return buffer;
}

char **sh_parse_line(char *line)
{
    int bufsize = LINE_NUM_ARGS;
    char **args = malloc_or_exit(bufsize * sizeof(char *));
    int i = 0;

    char *arg = strtok(line, LINE_DELIMITERS);
    while (arg != NULL)
    {
        args[i++] = arg;
        if (i >= bufsize)
        {
            bufsize *= 2;
            args = realloc_or_exit(args, bufsize * sizeof(char *));
        }
        arg = strtok(NULL, LINE_DELIMITERS);
    }
    args[i] = NULL;
    return args;
}

FILE *sh_get_new_stdout(char **args)
{
    int i = 0;
    FILE *new_stdout = NULL;
    while (args[i] != NULL)
    {
        if (strcmp(args[i], ">") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "No filename found for stdout redirection\n");
                printf("Syntax error\n");
                return NULL;
            }
            new_stdout = fopen(args[i + 1], "w");
            args[i] = NULL;
            break;
        }
        ++i;
    }
    return new_stdout;
}

void sh_execute(char **args, FILE *new_stdout)
{
    if (strcmp(args[0], "exit") == 0)
    {
        exit(EXIT_SUCCESS);
    }

    pid_t pid = fork();
    int status;

    // Child process
    if (pid == 0)
    {
        if (new_stdout != NULL)
            dup2(fileno(new_stdout), STDOUT_FILENO);

        execvp(args[0], args);
        fprintf(stderr, "execvp error\n");
        exit(EXIT_FAILURE);
    }
    // Parent process
    else if (pid > 0)
    {
        waitpid(pid, &status, WUNTRACED);
    }
    // Error
    else
    {
        fprintf(stderr, "fork failure\n");
        exit(EXIT_FAILURE);
    }
}