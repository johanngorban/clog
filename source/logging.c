#include "logging.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LOG_LENGTH 1024
#define MAX_LINE_NUMBER_LENGTH 5

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

    char line_number[MAX_LINE_NUMBER_LENGTH];
    sprintf(line_number, "%d", line);
    
    char result[MAX_LOG_LENGTH];
    if (result == NULL) {
        return NULL;
    }

    sprintf(result, "%-17s %s line: %s %-8s %s\n", time_buffer, file, line_number, type_str, message);
    
    free(type_str);
    return result;
}

void logger_(FILE *dest, enum LoggingType type, const char *message, const char *file, const size_t line) {
    if (dest == NULL) return;

    time_t raw_time;
    time(&raw_time);

    const char *note = create_log(type, message, &raw_time, file, line);
    if (note == NULL) {
        return;
    }

    fprintf(dest, "%s", note);
    free(note);
}
