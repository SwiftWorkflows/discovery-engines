/*
 * checks.h
 *
 *  Created on: Mar 5, 2014
 *      Author: wozniak
 */

#ifndef CHECKS_H
#define CHECKS_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED __attribute__((unused))

UNUSED
static void
crash(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}

UNUSED
static void
file_not_found(const char* filename)
{
    printf("Could not find file: %s\n", filename);
    fflush(NULL);
    exit(EXIT_FAILURE);
}

UNUSED
static void
file_not_writable(const char* filename)
{
    printf("Could not write to file: %s\n", filename);
    fflush(NULL);
    exit(EXIT_FAILURE);
}

UNUSED
static void
check_msg_impl(const char* format, ...)
{
  int buffer_size = 10240;
  char buffer[buffer_size];
  int count = 0;
  char* p = &buffer[0];
  va_list ap;
  va_start(ap, format);
  count += sprintf(p, "mt2: error: ");
  count += vsnprintf(buffer+count, (size_t)(buffer_size-count),
                     format, ap);
  va_end(ap);
  printf("%s\n", buffer);
  fflush(NULL);
  exit(1);
}

/** Nice vargs error check and message */
#define check_msg(condition, format, args...)  \
    { if (!(condition))                          \
       check_msg_impl(format, ## args);        \
    }

#endif
