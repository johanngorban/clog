#pragma once

#include <stdio.h>
#include <stddef.h>

typedef enum {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
} log_type_t;

typedef enum {
    LOG_QUIET   = 0,
    LOG_DEFAULT = 1,
    LOG_VERBOSE = 2,
    LOG_DEBUG   = 3
} log_level_t;

typedef void (*log_printer)(const char *message, void *context);

typedef void (*log_closer)(void *context);

typedef struct {
    log_printer print;
    log_closer close; 
    void *context;
} log_destinations_t;