#pragma once

#include "types.h"
#include <time.h>
#include <stdio.h>

void logging_write(FILE *file, enum LoggingType type, const char *message);
