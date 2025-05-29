#include "logging.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

static void stdout_write(const char *note);

static void file_write(FILE *file, const char *note);

static const char *note_to_str(enum LoggingType type, const char *message, const time_t *time);

void logging_write(FILE *file, enum LoggingType type, const char *message) {
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
    case Debug:
        message = "[DEBUG]";
        break;
    case Info:
        message = "[INFO]";
        break;
    case Warning:
        message = "[WARNING]";
        break;
    case Error:
        message = "[ERROR]";
        break;
    case Fatal:
        message = "[FATAL]";
        break;
    default:
        message = "[UNKNOWN]";
        break;
    }

    return message;
}

static const char *note_to_str(enum LoggingType type, const char *message, const time_t *time) {
    const char *time_str = ctime(time);
    const char *type_str = get_type_name(type);
    
    char *clean_time = strdup(time_str);
    if (clean_time) {
        clean_time[strlen(clean_time) - 1] = '\0';
    }

    size_t len = strlen(clean_time) + strlen(type_str) + strlen(message) + 3;
    char *result = (char *) malloc(len);
    if (result == NULL) {
        free(clean_time);
        return NULL;
    }

    snprintf(result, len, "%s %s %s", clean_time, type_str, message);

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