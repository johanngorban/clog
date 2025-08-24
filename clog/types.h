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
} log_type_t;


typedef void (*log_printer)(const char *message, void *context);

typedef void (*log_closer)(void *context);

typedef struct {
    log_printer print;
    log_closer close; 
    void *context;
} log_destinations_t;