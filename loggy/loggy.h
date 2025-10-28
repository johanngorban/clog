#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

#define log_debug(fmt, ...)   _loggy(DEBUG,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)    _loggy(INFO,   __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) _loggy(WARNING,__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...)   _loggy(ERROR,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...)   _loggy(FATAL,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define set_log_debug()             set_log_level(DEBUG)
#define set_log_info()              set_log_level(INFO)
#define set_log_warning()           set_log_level(WARNING)
#define set_log_error()             set_log_level(ERROR)
#define set_log_fatal_only()        set_log_level(FATAL)
#define set_log_log_level(l)        set_log_level(l)

#define LOG_DATE 0
#define LOG_TIME 1
#define LOG_PATH 2
#define LOG_LINE 4
#define LOG_SHORT   LOG_DATE | LOG_TIME
#define LOG_FULL    LOG_DATE | LOG_TIME | LOG_PATH | LOG_LINE

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


int log_stdout_append();

int log_file_append(const char *path);

void set_log_flags(unsigned int flags);

void set_log_level(log_level_t level);

void _loggy(
    log_level_t level, 
    const char *file, 
    const size_t line, 
    const char *fmt, ...
);

void log_exit();

#ifdef __cplusplus
}
#endif