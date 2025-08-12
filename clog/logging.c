#include "logging.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LOG_LENGTH 1024
#define MAX_LINE_NUMBER_LENGTH 5

static LoggingDestinations logging_dests;

static const char *get_type_name(LoggingType type);

static const char *create_log(LoggingType type, const char *message, const time_t *time, const char *file, const size_t line);

static void print_log(const char *log);

// General functions

size_t log_init_(size_t args, ...) {
    va_list ap;
    va_start(ap, args);

    for (size_t i = 0; i < args; i++) {
        char *path = va_arg(ap, char *);
        if (path) {
            log_file_append(path);
        }
    }

    va_end(ap);

    atexit(log_exit);
    return logging_dests.current_dest_count;
}

void logger_(LoggingType type, const char *message, const char *file, const size_t line) {
    time_t raw_time;
    time(&raw_time);

    const char *log = create_log(type, message, &raw_time, file, line);
    if (log == NULL) {
        return;
    }

    print_log(log);

    return;
}

int log_file_append(const char *path) {
    if (logging_dests.current_dest_count >= MAX_DESTINATION_COUNT) {
        return 0;
    }
    FILE *log_file = fopen(path, "a");
    if (log_file) {
        size_t current_index = logging_dests.current_dest_count;
        logging_dests.dest[current_index] = log_file;
        logging_dests.current_dest_count++;
        
        return 1;
    }

    return 0;
}

void log_exit() {
    for (size_t i = 0; i < logging_dests.current_dest_count; i++) {
        if (logging_dests.dest[i]) {
            fclose(logging_dests.dest[i]);
        }
    }
}

// ----------------------------------------

static void print_log(const char *log) {
    for (size_t i = 0; i < logging_dests.current_dest_count; i++) {
        if (logging_dests.dest[i]) {
            fprintf(logging_dests.dest[i], "%s", log);
            fflush(logging_dests.dest[i]);
        }
    }
}

static const char *create_log(LoggingType type, const char *message, const time_t *time, const char *file, const size_t line) {
    struct tm *timeinfo = localtime(time);
    char time_buffer[20];
    strftime(time_buffer, sizeof(time_buffer), "%d/%m/%y %H:%M:%S", timeinfo);

    const char *type_str = get_type_name(type);

    char line_number[MAX_LINE_NUMBER_LENGTH];
    sprintf(line_number, "%d", line);
    
    char *log = malloc(MAX_LOG_LENGTH);
    if (log == NULL) {
        return NULL;
    }

    sprintf(log, "%-17s %s line: %s %-8s %s\n", time_buffer, file, line_number, type_str, message);
    
    return log;
} 

static const char *get_type_name(LoggingType type) {
    char *message;
    switch (type) {
    case DEBUG:     message = "[DEBUG]"; break;
    case INFO:      message = "[INFO]"; break;
    case WARNING:   message = "[WARNING]"; break;
    case ERROR:     message = "[ERROR]"; break;
    case FATAL:     message = "[FATAL]"; break;
    default:        message = "[UNKNOWN]"; break;
    }

    return message;
}