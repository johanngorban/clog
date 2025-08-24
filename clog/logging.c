#include "logging.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_LOG_LENGTH 1024
#define MAX_LINE_NUMBER_LENGTH 5


/*
* Main objects and flags
*/
static bool logging_init = false;

static log_destinations_t logs;

pthread_mutex_t logs_mutex;

/*
* Helper functions
*/
static const char *get_type_name(log_type_t type);

static const char *create_log(log_type_t type, const char *message, const time_t *time, const char *file, const size_t line);

static void print_log(const char *log);

/*
* General functions
*/
int log_init_(size_t args, ...) {
    if (logging_init == true) {
        return 0;
    }

    pthread_mutex_init(&logs_mutex, NULL);

    va_list ap;
    va_start(ap, args);

    for (size_t i = 0; i < args; i++) {
        char *path = va_arg(ap, char *);
        if (path) {
            log_file_append(path);
        }
    }

    logging_init = true;

    va_end(ap);

    atexit(log_exit);
    return logs.current_dest_count;
}

void logger_(log_type_t type, const char *message, const char *file, const size_t line) {
    time_t raw_time;
    time(&raw_time);

    const char *log = create_log(type, message, &raw_time, file, line);
    if (log == NULL) {
        return;
    }

    print_log(log);

    free(log);

    return;
}

int log_file_append(const char *path) {
    int result = 0;
    if (logs.current_dest_count >= MAX_DESTINATION_COUNT) {
        return result;
    }

    pthread_mutex_lock(&logs_mutex);
    
    FILE *log_file = fopen(path, "a");
    if (log_file) {
        size_t current_index = logs.current_dest_count;
        logs.dest[current_index] = log_file;
        logs.current_dest_count++;
        
        result++;
    }

    pthread_mutex_unlock(&logs_mutex);

    return result;
}

void log_exit() {
    pthread_mutex_lock(&logs_mutex);

    for (size_t i = 0; i < logs.current_dest_count; i++) {
        if (logs.dest[i]) {
            fclose(logs.dest[i]);
        }
    }
    logs.current_dest_count = 0;
    logging_init = false;

    pthread_mutex_unlock(&logs_mutex);

    pthread_mutex_destroy(&logs_mutex);
}


/*
* Helper functions
*/
static void print_log(const char *log) {
    pthread_mutex_lock(&logs_mutex);

    for (size_t i = 0; i < logs.current_dest_count; i++) {
        if (logs.dest[i]) {
            fprintf(logs.dest[i], "%s", log);
            fflush(logs.dest[i]);
        }
    }

    pthread_mutex_unlock(&logs_mutex);
}

static const char *create_log(log_type_t type, const char *message, const time_t *time, const char *file, const size_t line) {
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

static const char *get_type_name(log_type_t type) {
    char *message;
    switch (type) {
    case DEBUG:     
        message = "[DEBUG]"; 
        break;
    case INFO: 
        message = "[INFO]"; 
        break;
    case WARNING: 
        message = "[WARNING]"; 
        break;
    case ERROR:
        message = "[ERROR]"; 
        break;
    case FATAL:
        message = "[FATAL]"; 
        break;
    default:
        message = "[UNKNOWN]"; 
        break;
    }

    return message;
}