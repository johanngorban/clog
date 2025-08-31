#include "unity.h"
#include "logging.h"

void test_before_init() {
    log_info("This message should be not displayed");
    log_error("This message should be not displayed");
}

void test_log_init_valid() {
    log_init("logs");

    TEST_SOURCE_FILE("logs");
}

void test_append_files() {
    log_file_append("other_logs");
    log_file_append("tests/other_logs");

    TEST_SOURCE_FILE("logs");
    TEST_SOURCE_FILE("tests/other_logs");
    TEST_SOURCE_FILE("other_logs");
}

void test_invalid_path() {
    log_file_append("invalid_dir/log");
    TEST_SOURCE_FILE("invalid_dir/log");
}

void test_debug_logs() {
    log_debug("Some debug tests with time and line in code and source file");
    log_debug("Other debug text");
}

void test_info_logs() {
    log_info("Some info text");
    log_info("Some other debug text");
}

void test_fatal_logs() {
    log_fatal("Some fatal text");
    log_fatal("Some other fatal text");
}

void test_warning_logs() {
    log_warning("Some warnings");
    log_warning("Some other warnings");
}

void test_error_logs() {
    log_error("Some error");
    log_error("Some other error");
}

void test_log_close() {
    log_exit();
}