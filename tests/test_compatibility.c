#include "unity.c"
#include "logging.h"

void test_compatibility() {
    log_init("def_path");

    log_info("Test message");

    log_file_append("New_path");

    log_info("New path addded");
}