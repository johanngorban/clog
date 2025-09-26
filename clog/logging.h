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

#define set_quiet()             set_log_level(LOG_QUIET)
#define set_default()           set_log_level(LOG_DEFAULT)
#define set_verbose()           set_log_level(LOG_VERBOSE)


/*
*   Interface functions
*/
int log_file_append(const char *path);

void set_log_level(log_level_t level);

void __log(log_type_t type, const char *message, const char *file, const size_t line);

int __log_init(size_t args, ...); 

int __log_allowed(log_type_t type, log_level_t level);

void log_exit();