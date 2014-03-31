/*
 * checks.h
 *
 *  Created on: Mar 5, 2014
 *      Author: wozniak
 */

#ifndef CHECKS_H
#define CHECKS_H

#include <stdio.h>
#include <stdlib.h>

static void file_not_found(const char* filename)
{
    printf("Could not find file: %s\n", filename);
    fflush(NULL);
    exit(EXIT_FAILURE);
}

static void file_not_writable(const char* filename)
{
    printf("Could not write to file: %s\n", filename);
    fflush(NULL);
    exit(EXIT_FAILURE);
}

#endif
