/*
 *  Helper functions for q3.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

void trim(char *line)
{
    int size = strlen(line);

    // If nothing to trim
    if (!isspace(line[0]) && !isspace(line[size - 1]))
        return;

    int start, end;

    // Find start
    for (int i = 0; i < size; ++i)
    {
        if (isspace(line[i]))
            continue;
        start = i;
        break;
    }

    // Find end
    for (int i = 0; i < size; ++i)
    {
        if (!isspace(line[i]))
            end = i;
    }

    // Shift line
    for (int i = 0; i <= end - start + 1; ++i)
    {
        line[i] = line[start + i];
        if (i == end - start + 1)
            line[i] = '\0';
    }
}

void *malloc_or_exit(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "malloc failure\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void *realloc_or_exit(void *ptr, size_t size)
{
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL)
    {
        fprintf(stderr, "realloc failure\n");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}