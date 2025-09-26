#include "logging.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_LOG_LENGTH 1024
#define MAX_LINE_NUMBER_LENGTH 5
#define MAX_DESTINATION_COUNT 1024


/*
* Main objects and flags
*/
static bool logging_init = false;

static log_level_t log_level = LOG_DEFAULT;

static log_destinations_t log_destinations[MAX_DESTINATION_COUNT];
static size_t log_dest_counter = 0;

pthread_mutex_t logs_mutex;

/*
* Helper functions
*/
static const char *get_type_name(log_type_t type);

static const char *create_log(log_type_t type, const char *message, const time_t *time, const char *file, const size_t line);

static void print_log(const char *log);

// File sinks

static void file_log_print(const char *message, void *context) {
    FILE *f = (FILE *) context;

    if (f == NULL) {
        return;
    }

    fprintf(f, "%s", message);
    fflush(f);
}

static void file_close(void *context) {
    FILE *f = (FILE *) context;
    if (f != NULL) {
        fflush(f);
        fclose(f);
    }
}

int log_file_append(const char *path) {
    if (log_dest_counter >= MAX_DESTINATION_COUNT) {
        return 0;
    } 

    FILE *f = fopen(path, "a");

    if (f == NULL) {
        return 0;
    }

    log_destinations[log_dest_counter].print = file_log_print;
    log_destinations[log_dest_counter].close = file_close;
    log_destinations[log_dest_counter].context = f;

    log_dest_counter++;
    return 1;
}

// Stdout sinks

static void stdout_log_print(const char *message, void *context) {
    (void) context;
    printf("%s", message);
}

int log_stdout_append() {
    if (log_dest_counter >= MAX_DESTINATION_COUNT) {
        return 0;
    }

    log_destinations[log_dest_counter].print = stdout_log_print;
    log_destinations[log_dest_counter].close = NULL;
    log_destinations[log_dest_counter].context = NULL;

    log_dest_counter++;

    return 1;
}

/*
* General functions
*/
int __log_init(size_t args, ...) {
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

    return 1;
}

void __log(log_type_t type, const char *message, const char *file, const size_t line) {
    if (!__log_allowed(type, log_level)) {
        return;
    }

    time_t raw_time;
    time(&raw_time);

    const char *log = create_log(type, message, &raw_time, file, line);
    if (log == NULL) {
        return;
    }

    print_log(log);

    free((void *) log);

    return;
}

void log_exit() {
    pthread_mutex_lock(&logs_mutex);

    for (size_t i = 0; i < log_dest_counter; i++) {
        if (log_destinations[i].close != NULL) {
            log_destinations[i].close(log_destinations[i].context);
        }
    }

    log_dest_counter = 0;
    logging_init = false;

    pthread_mutex_unlock(&logs_mutex);

    pthread_mutex_destroy(&logs_mutex);
}


/*
* Helper functions
*/
static void print_log(const char *log) {
    pthread_mutex_lock(&logs_mutex);

    for (size_t i = 0; i < log_dest_counter; i++) {
        log_destinations[i].print(log, log_destinations[i].context);
    }

    pthread_mutex_unlock(&logs_mutex);
}

static const char *create_log(log_type_t type, const char *message, const time_t *time, const char *file, const size_t line) {
    struct tm *timeinfo = localtime(time);
    char time_buffer[20];
    strftime(time_buffer, sizeof(time_buffer), "%d/%m/%y %H:%M:%S", timeinfo);

    const char *type_str = get_type_name(type);

    char line_number[MAX_LINE_NUMBER_LENGTH];
    sprintf(line_number, "%zu", line);
    
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

int __log_allowed(log_type_t type, log_level_t level) {
    switch (type) {
    case WARNING:
        return level >= LOG_DEFAULT;
    case INFO:
        return level >= LOG_VERBOSE;
    case DEBUG:
        return level >= LOG_DEBUG;
    case ERROR:
        return level >= LOG_QUIET;
    case FATAL:
        return level >= LOG_QUIET;
    }

    return 0;
}