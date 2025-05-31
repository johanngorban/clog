#pragma once

#include "types.h"
#include <time.h>
#include <stdio.h>

#define logger(destination_, type_, message_, file_, line_) logger_(destination_, type_, message_, file_, line_)

#define log_debug(destination_, message_) logger_(destination_, DEBUG, message_, __FILE__, __LINE__)

#define log_info(destination_, message_) logger_(destination_, INFO, message_, __FILE__, __LINE__)

#define log_warning(destination_, message_) logger_(destination_, WARNING, message_, __FILE__, __LINE__)

#define log_error(destination_, message_) logger_(destination_, ERROR, message_, __FILE__, __LINE__)

#define log_fatal(destination_, message_) logger_(destination_, FATAL, message_, __FILE__, __LINE__)

/* General function */

void logger_(FILE *dest, enum LoggingType type, const char *message, const char *file, const size_t line);