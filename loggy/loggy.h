#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define log_debug(fmt, ...)   _loggy(DEBUG,     fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)    _loggy(INFO,      fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) _loggy(WARNING,   fmt, ##__VA_ARGS__)
#define log_error(fmt, ...)   _loggy(ERROR,     fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...)   _loggy(FATAL,     fmt, ##__VA_ARGS__)

#define LOG_NONE        (0)
#define LOG_DATE        (1 << 0)
#define LOG_TIME        (1 << 1)
#define LOG_DEFAULT     (LOG_DATE | LOG_TIME)

typedef enum {
    FATAL   = 0,
    ERROR   = 1,
    INFO    = 2,
    WARNING = 3,
    DEBUG   = 4,
} log_level_t;

typedef void (*log_printer)(const char *message, void *context);

typedef void (*log_closer)(void *context);

typedef struct {
    log_printer print;
    log_closer close; 
    void *context;
} log_destinations_t;

int log_init(
    log_level_t level, 
    uint8_t flags
);

void log_shutdown();

int log_stdout_append();

int log_file_append(const char *path);

void set_log_flags(unsigned int flags);

void set_log_level(log_level_t level);

void _loggy(
    log_level_t level, 
    const char *fmt, ...
);



#ifdef __cplusplus
}
#endif