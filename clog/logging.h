#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include <stdio.h>
#include <stdarg.h>

#define log_init(...) __clog_init(sizeof (char *[]) {__VA_ARGS__} / sizeof (char *), __VA_ARGS__)

#define logger(level_, fmt, ...) __clog(level_, __FILE__, __LINE__, fmt, ...)

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

/*
*   Interface functions
*/
int log_file_append(const char *path);

void __set_clog_level(log_level_t level);

void __clog(log_level_t level, const char *file, const size_t line, const char *fmt, ...);

int __clog_init(size_t args, ...); 

void log_exit();

#ifdef __cplusplus
}
#endif