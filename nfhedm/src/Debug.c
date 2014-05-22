
/*
 * Debug.c
 *
 *  Created on: May 21, 2014
 *      Author: wozniak
 */

#include <stdlib.h>
#include <string.h>

#include "Debug.h"

bool debug_enabled   = true;
bool log_enabled     = true;
bool profile_enabled = true;

static double start = -1.0;

static inline double start_time(void)
{
    if (start == -1.0)
        start = time_double();
    return start;
}

static inline double walltime(void)
{
    double start = start_time();
    return time_double() - start;
}

void
debug_printf(const char *token, const char *format, ...)
{
    double wt = walltime();
    printf(" %8.5f %s", wt, token);
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}

static int profile_index = 0;

#if ENABLE_PROFILE

#define PROFILE_ENTRIES 1024

typedef struct {
    char token[PROFILE_TOKEN_LENGTH];
    double start;
    double stop;
} profile_entry;

static profile_entry profile_array[PROFILE_ENTRIES];

int
profile_create(const char* token)
{
    if (!profile_enabled)
        return 0;

    if (strlen(token) > PROFILE_TOKEN_LENGTH-1)
    {
        printf("Profile token too long (max %i): %s\n",
               PROFILE_TOKEN_LENGTH, token);
        exit(1);
    }
    if (profile_index == PROFILE_ENTRIES)
    {
        printf("Too many profile entries!\n");
        exit(1);
    }

    int i = profile_index;
    strcpy(profile_array[i].token, token);
    profile_array[i].start = 0.0;
    profile_array[i].stop  = 0.0;
    profile_index++;
    return i;
}

void
profile_start(int i)
{
    if (!profile_enabled)
        return;
    profile_array[i].start = time_double();
}

void
profile_stop(int i)
{
    if (!profile_enabled)
        return;
    profile_array[i].stop = time_double();
}

void
profile_report()
{
    if (!profile_enabled)
        return;
    for (int i = 0; i < profile_index; i++)
    {
        if (profile_array[i].start == 0.0)
        {
            printf("PROFILE: %-*s: DID_NOT_START\n",
                   PROFILE_TOKEN_LENGTH, profile_array[i].token);
            continue;
        }
        if (profile_array[i].stop == 0.0)
        {
            printf("PROFILE: %-*s: DID_NOT_STOP\n",
                   PROFILE_TOKEN_LENGTH, profile_array[i].token);
            continue;
        }
        double duration =
                profile_array[i].stop - profile_array[i].start;
        printf("PROFILE: %-*s: %0.6f\n",
               PROFILE_TOKEN_LENGTH, profile_array[i].token, duration);
    }
}

void
profile_reset()
{
    if (!profile_enabled)
        return;
    profile_index = 0;
}

#endif // ENABLE_PROFILE
