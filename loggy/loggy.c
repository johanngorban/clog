// ============================================================
//  loggy.c — Implementation of logging library
// ============================================================


// --------------------- Includes ---------------------
#include "loggy.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// --------------------- Constants ---------------------
#define MAX_LOG_LENGTH  (1 << 7)
#define MAX_SINK_COUNT  (8)


// --------------------- Internal State ---------------------
static uint8_t log_initialized = 0;
static log_level_t log_level = INFO;
static uint8_t log_flags = LOG_DEFAULT;

static log_sink_t log_sinks[MAX_SINK_COUNT];
static uint8_t sink_count = 0;

pthread_mutex_t log_mutex;

// --------------------- Helpers (static) ---------------------
static const char *get_level_name(log_level_t level);

static const char *create_log(
    log_level_t level, 
    const char *message, 
    const time_t *time
);

static void print_log(const char *log);

static void log_file_printer (
    const char *message, 
    void *context
);

static void log_stdout_printer (
    const char *message, 
    void *context
);

static void log_file_closer(void *context);

// --------------------- Initialization ---------------------
int8_t log_init(log_level_t level, uint8_t flags) {
    if (log_initialized != 0) {
        // Already initialized
        return 0;
    }
    
    if (level > DEBUG || level < FATAL) {
        return -1;
    }

    pthread_mutex_init(&log_mutex, NULL);
    log_set_flags(flags); // log_flags = flags;
    log_set_level(level); // log_level = level;

    log_initialized = 1;

    return 0;
}

void log_shutdown() {
    pthread_mutex_lock(&log_mutex);

    for (size_t i = 0; i < sink_count; i++) {
        if (log_sinks[i].close != NULL) {
            log_sinks[i].close(log_sinks[i].context);
        }
    }

    sink_count = 0;

    pthread_mutex_unlock(&log_mutex);
    pthread_mutex_destroy(&log_mutex);

    log_initialized = 0;
}

void log_set_level(log_level_t level) {
    if (level > DEBUG || level < FATAL) {
        return;
    }

    log_level = level;
}

void log_set_flags(uint8_t flags) {
    log_flags = flags;
}

// --------------------- Logging ---------------------
void log_debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message_va(DEBUG, fmt, args);
    va_end(args);
}

void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message_va(INFO, fmt, args);
    va_end(args);
}

void log_warning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message_va(WARNING, fmt, args);
    va_end(args);
}

void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message_va(ERROR, fmt, args);
    va_end(args);
}

void log_fatal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_message_va(FATAL, fmt, args);
    va_end(args);
}

void log_message_va(
    log_level_t level, 
    const char *fmt,
    va_list args
) {
    if (log_initialized == 0 || level > log_level) {
        return;
    }

    char message[MAX_LOG_LENGTH];
    
    vsnprintf(message, sizeof(message), fmt, args);

    time_t raw_time;
    time(&raw_time);

    const char *log = create_log(level, message, &raw_time);
    if (log == NULL) {
        return;
    }

    print_log(log);
}

// --------------------- Sink Management ---------------------
int8_t log_add_custom_sink(
    log_printer printer,
    log_closer closer,
    void *ctx
) {
    pthread_mutex_lock(&log_mutex);
    if (sink_count >= MAX_SINK_COUNT) {
        pthread_mutex_unlock(&log_mutex);
        return -1;
    }

    log_sinks[sink_count++] = (log_sink_t) {
        printer,
        closer,
        ctx
    };

    pthread_mutex_unlock(&log_mutex);

    return 0;
}

int8_t log_add_file_sink(const char *path) {
    FILE *file = fopen(path, "a");
    if (file == NULL) {
        return -1;
    }

    return log_add_custom_sink(
        log_file_printer,
        log_file_closer,
        file
    );
}

int8_t log_add_stdout_sink() {
    return log_add_custom_sink(
        log_stdout_printer,
        NULL,
        NULL
    );
}

static void log_file_printer (
    const char *message, 
    void *context
) {
    FILE *f = (FILE *) context;
    if (f == NULL) {
        return;
    }

    fprintf(f, "%s", message);
    fflush(f);
}

static void log_file_closer(void *context) {
    FILE *f = (FILE *) context;
    if (f != NULL) {
        fflush(f);
        fclose(f);
    }
}

static void log_stdout_printer (
    const char *message, 
    void *context
) {
    (void) context;
    printf("%s", message);
}

// --------------------- Helpers (static) ---------------------

static void print_log(const char *log) {
    pthread_mutex_lock(&log_mutex);

    for (size_t i = 0; i < sink_count; i++) {
        log_sinks[i].print(log, log_sinks[i].context);
    }

    pthread_mutex_unlock(&log_mutex);
}

static const char *create_log(
    log_level_t level, 
    const char *message, 
    const time_t *time
) {
    static char log_buffer[MAX_LOG_LENGTH];

    char date_part[16] = "";
    char time_part[16] = "";

    if (time != NULL) {
        struct tm timeinfo;
        struct tm *tmp = localtime(time);
        if (tmp) {
            timeinfo = *tmp;
        } 
        else {
            memset(&timeinfo, 0, sizeof(timeinfo));
        }

        if (log_flags & LOG_DATE) {
            strftime(date_part, sizeof(date_part), "%d/%m/%y", &timeinfo);
        }

        if (log_flags & LOG_TIME) {
            strftime(time_part, sizeof(time_part), "%H:%M:%S", &timeinfo);
        }
    }

    const char *level_str = get_level_name(level);

    uint8_t offset = 0;
    uint8_t written = 0;

    log_buffer[0] = '\0';

    if (log_flags & LOG_DATE && date_part[0]) {
        written = snprintf(
            log_buffer + offset, 
            MAX_LOG_LENGTH - offset, 
            "%s ", date_part
        );
        if (written < 0) {
            return NULL;
        }

        offset += written;
    }
    if (log_flags & LOG_TIME && time_part[0]) {
        written = snprintf(
            log_buffer + offset,
            MAX_LOG_LENGTH - offset,
            "%s ", time_part
        );
        if (written < 0) {
            return NULL;
        }
        
        offset += written;
    }

    snprintf(
        log_buffer + offset,
        MAX_LOG_LENGTH - offset,
        "%-8s %s\n",
        level_str,
        message
    );

    return log_buffer;
} 

static const char *get_level_name(log_level_t level) {
    char *message;
    switch (level) {
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