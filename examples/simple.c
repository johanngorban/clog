#include <stdio.h>
#include "logging.h"

int main() {
    log_init("logs");

    // Here is other program parts
    printf("Do something");
    log_info("printf() called");
    // ...

    printf("Error occurred!");
    log_error("printf() says an error occurred");
    // ...


    // If fatal error occurres, do not forget to exit all log
    printf("Fatal error: program crashed!!! CODE: 0x232");
    log_fatal("Fatal error with code 0x232!");
    // ...

    log_exit();
}