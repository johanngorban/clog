#include <loggy.h>

int main() {
    log_init(INFO, LOG_DEFAULT);

    log_add_stdout_sink(); // Write in stdout

    log_info("Start the program");
    for (int i = 0; i < 128; i++) {
        if (i % 32 == 0) {
            log_error("%d mod 32 == 0", i);
        }
        else {
            log_info("Current number is %d", i);
        }
    }

    log_info("End the program");

    return 0;
}