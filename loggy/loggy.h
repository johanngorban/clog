/*
==============================================================
    Loggy — Lightweight C Logging Library
==============================================================

    Cross-platform logging library for C and C++ projects.
    Supports multiple output sinks (stdout, file, custom),
    configurable log levels and formatting flags.

    Author: Ivan Gorban
    License: MIT
    Repository: https://github.com/johanngorban/loggy
    Version: 1.3.1
==============================================================
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define LOG_NONE        (0)         // No date and time
#define LOG_DATE        (1 << 0)    // Add date dd/mm/yy
#define LOG_TIME        (1 << 1)    // Add time hh/mm/ss
#define LOG_DEFAULT     (LOG_TIME)  // Add time
#define LOG_FULL        (LOG_TIME | LOG_DATE) // Add date and time

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
} log_sink_t;

// --------------------- Initialization ---------------------
int8_t log_init(
    log_level_t level, 
    uint8_t flags
);
void log_shutdown();

void log_set_level(log_level_t level);
void log_set_flags(uint8_t flags);

// --------------------- Sink Management ---------------------
int8_t log_add_stdout_sink();
int8_t log_add_file_sink(const char *path);
int8_t log_add_custom_sink(
    log_printer printer,
    log_closer closer,
    void *ctx
);

// --------------------- Logging ---------------------
void log_debug(const char *fmt, ...);
void log_info(const char *fmt, ...);
void log_warning(const char *fmt, ...);
void log_error(const char *fmt, ...);
void log_fatal(const char *fmt, ...);
void log_message_va(
    log_level_t level, 
    const char *fmt,
    va_list args
);

#ifdef __cplusplus
}
#endif