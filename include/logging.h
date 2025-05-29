#pragma once

#include "types.h"
#include <time.h>
#include <stdio.h>

void logger(FILE *file, enum LoggingType type, const char *message);
