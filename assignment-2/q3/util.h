#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

/*
 *  Removes any leading or trailing whitespaces (' ', '\t', '\n')
 */
void trim(char *line);

/*
 *  Same as malloc, but exits if malloc fails
 */
void *malloc_or_exit(size_t size);

/*
 *  Same as realloc, but exits if realloc fails
 */
void *realloc_or_exit(void *ptr, size_t size);

#endif