#pragma once

#include "types.h"
#include <stdio.h>
#include <stdarg.h>

#define log_init(...) __log_init(sizeof (char *[]) {__VA_ARGS__} / sizeof (char *), __VA_ARGS__)

#define logger(type_, message_) __log(type_, message_, __FILE__, __LINE__)

#define log_debug(message_)     __log(DEBUG, message_, __FILE__, __LINE__)
#define log_info(message_)      __log(INFO, message_, __FILE__, __LINE__)
#define log_warning(message_)   __log(WARNING, message_, __FILE__, __LINE__)
#define log_error(message_)     __log(ERROR, message_, __FILE__, __LINE__)
#define log_fatal(message_)     __log(FATAL, message_, __FILE__, __LINE__)

#define set_debug()             __set_log_level(DEBUG)
#define set_info()              __set_log_level(INFO)
#define set_warning()           __set_log_level(WARNING)
#define set_error()             __set_log_level(ERROR)
#define set_fatal_only()        __set_log_level(FATAL)

/*
*   Interface functions
*/
int log_file_append(const char *path);

void __set_log_level(log_type_t level);

void __log(log_type_t type, const char *message, const char *file, const size_t line);

int __log_init(size_t args, ...); 

void log_exit();