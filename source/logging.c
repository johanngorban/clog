#include "logging.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>

static const char *get_type_name(enum LoggingType type) {
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

static const char *create_log(enum LoggingType type, const char *message, const time_t *time, const char *file, const size_t line) {
    struct tm *timeinfo = localtime(time);
    char time_buffer[20];
    strftime(time_buffer, sizeof(time_buffer), "%d/%m/%y %H:%M:%S", timeinfo);

    const char *type_str = get_type_name(type);

    char *line_number = (char *) malloc(5);
    sprintf(line_number, "%d", line);
    
    size_t len = strlen(file) + strlen(line_number) + strlen(time_buffer) + strlen(type_str) + strlen(message) + 5; // +2 пробела + \0
    char *result = (char *) malloc(len);
    if (result == NULL) {
        return NULL;
    }

    snprintf(result, len, "%s %s %s %s %s", time_buffer, file, line_number,type_str, message);
    return result;
}

static void dest_write(FILE *dest, const char *note) {
    if (dest) {
        fprintf(dest, "%s\n", note);
    }
}

static void stdout_write(const char *note) {
    fprintf(stdout, "%s\n", note);
}

void logger_(FILE *dest, enum LoggingType type, const char *message, const char *file, const size_t line) {
    if (dest == NULL) return;

    time_t raw_time;
    time(&raw_time);

    const char *note = create_log(type, message, &raw_time, file, line);
    if (note == NULL) {
        return;
    }

    if (dest == stdout) {
        stdout_write(note);
    } 
    else {
        dest_write(dest, note);
    }
}
