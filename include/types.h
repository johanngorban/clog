#pragma once

#include <stdio.h>
#include <stddef.h>

#define MAX_DESTINATION_COUNT 32

typedef enum {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
} LoggingType;

typedef struct {
    FILE *dest[MAX_DESTINATION_COUNT];
    size_t current_dest_count;
} LoggingDestinations;
