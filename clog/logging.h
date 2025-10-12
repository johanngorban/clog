#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>

#define log_debug(fmt, ...)   __clog(DEBUG,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)    __clog(INFO,   __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) __clog(WARNING,__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...)   __clog(ERROR,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...)   __clog(FATAL,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define set_debug()             __set_clog_level(DEBUG)
#define set_info()              __set_clog_level(INFO)
#define set_warning()           __set_clog_level(WARNING)
#define set_error()             __set_clog_level(ERROR)
#define set_fatal_only()        __set_clog_level(FATAL)
#define set_log_level(l)        __set_clog_level(l)

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

int set_clog_flags(unsigned int flags);

int _set_clog_level(log_level_t level);

void _clog(log_level_t level, const char *file, const size_t line, const char *fmt, ...);

void log_exit();

#ifdef __cplusplus
}
#endif