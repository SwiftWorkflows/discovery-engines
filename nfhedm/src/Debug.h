
/*
 * Debug.h
 *
 *  Created on: May 21, 2014
 *      Author: wozniak
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define ENABLE_LOG     1
#define ENABLE_PROFILE 1
#define ENABLE_DEBUG   0

extern bool log_enabled;
extern bool profile_enabled;
extern bool debug_enabled;

/**
   Get the current time as a double to microsecond resolution
 */
__attribute__((unused))
static double
time_double()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double result = (double)tv.tv_sec + 0.000001 * (double)tv.tv_usec;
  return result;
}

void debug_printf(const char *token, const char *format, ...);

#if ENABLE_LOG
#define LOG(format, args...)                                          \
  { if (debug_enabled)                                                \
         debug_printf("LOG:   ", format "\n", ## args);          \
       }
#else
#define LOG(format, args...) // noop
#endif


#if ENABLE_DEBUG && !defined(NDEBUG)
#define DEBUG(format, args...)                                        \
  { if (debug_enabled) {                                              \
         debug_printf("DEBUG: ", format "\n", ## args);                \
         fflush(stdout);                                              \
       } }
#else
#define DEBUG(format, args...) // noop
#endif

#define PROFILE_TOKEN_LENGTH 32
#if ENABLE_PROFILE
#define PROFILE_CREATE(token,i ) int i = profile_create(#token)
#define PROFILE_START(i)         profile_start(i)
#define PROFILE_STOP(i)          profile_stop(i)
#define PROFILE_REPORT           profile_report()
#define PROFILE_RESET            profile_reset()
int  profile_create(const char* token);
void profile_start(int i);
void profile_stop(int i);
void profile_report(void);
void profile_reset(void);
#else
#define PROFILE_CREATE(token,i) // noop
#define PROFILE_START(i)        // noop
#define PROFILE_STOP(i)         // noop
#define PROFILE_REPORT          // noop
#define PROFILE_RESET           // noop
#endif

#endif
