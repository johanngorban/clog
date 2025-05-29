#include "logging.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

static void stdout_write(const char *note);

static void file_write(FILE *file, const char *note);

static const char *note_to_str(enum LoggingType type, const char *message, const time_t *time);

void logger(FILE *file, enum LoggingType type, const char *message) {
    if (file == NULL) return;

    time_t raw_time;
    time(&raw_time);

    const char *note = note_to_str(type, message, &raw_time);
    if (note == NULL) {
        return;
    }

    if (file == stdout) {
        stdout_write(note);
    } 
    else {
        file_write(file, note);
    }
}

static const char *get_type_name(enum LoggingType type) {
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

static const char *note_to_str(enum LoggingType type, const char *message, const time_t *time) {
    struct tm *timeinfo = localtime(time);
    char time_buffer[20];
    strftime(time_buffer, sizeof(time_buffer), "%d/%m/%y %H:%M:%S", timeinfo);

    const char *type_str = get_type_name(type);
    
    size_t len = strlen(time_buffer) + strlen(type_str) + strlen(message) + 3; // +2 пробела + \0
    char *result = malloc(len);
    if (result == NULL) {
        return NULL;
    }

    snprintf(result, len, "%s %s %s", time_buffer, type_str, message);
    return result;
}

static void file_write(FILE *file, const char *note) {
    if (file) {
        fprintf(file, "%s\n", note);
    }
}

static void stdout_write(const char *note) {
    fprintf(stdout, "%s\n", note);
}