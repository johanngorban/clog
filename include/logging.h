#pragma once

#include "types.h"
#include <time.h>
#include <stdio.h>

void logger(FILE *file, enum LoggingType type, const char *message);

void log_debug(FILE *file, const char *message);

void log_info(FILE *file, const char *message);

void log_warning(FILE *file, const char *message);

void log_error(FILE *file, const char *message);

void log_fatal(FILE *file, const char *message);