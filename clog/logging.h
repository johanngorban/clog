#pragma once

#include "types.h"
#include <stdio.h>
#include <stdarg.h>

#define log_init(...) log_init_(sizeof (char *[]) {__VA_ARGS__} / sizeof (int), __VA_ARGS__)

#define logger(type_, message_) logger_(type_, message_, __FILE__, __LINE__)

#define log_debug(message_)     logger_(DEBUG, message_, __FILE__, __LINE__)

#define log_info(message_)      logger_(INFO, message_, __FILE__, __LINE__)

#define log_warning(message_)   logger_(WARNING, message_, __FILE__, __LINE__)

#define log_error(message_)     logger_(ERROR, message_, __FILE__, __LINE__)

#define log_fatal(message_)     logger_(FATAL, message_, __FILE__, __LINE__)


/*
*   Interface functions
*/
int log_file_append(const char *path);

void logger_(log_type type, const char *message, const char *file, const size_t line);

size_t log_init_(size_t args, ...); 

void log_exit();