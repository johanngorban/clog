# Logging library for C

A lightweight logging library for C projects, supporting multiple log levels and output to `stdout` or file.

---

## Features

- Multiple log levels: DEBUG, INFO, WARNING, ERROR, FATAL
- Timestamps in log messages
- Output to standard output or to file
- Simple integration into existing C projects

---

## Requirements

- C compiler with C11 support
- CMake 3.16 or newer

---

## Building

To build the static library using CMake:

```sh
git clone https://your.repo.url/clogs.git
cd clogs
mkdir build
cd build
cmake ..
make
```

After the build, the `clogs` static library will be available in the `build` directory.

---

## Using as a Library

### 1. Include in Your Project

Copy the `include/` and `source/` directories into your project, or add `clogs` as a submodule.

The other way you can use the library is to use it as a domain (DDD). That is, the repository is a domain you can include in your project

### 2. Build with CMake

Add the following lines to your `CMakeLists.txt`:

```cmake
add_subdirectory(clogs)

target_link_libraries(your_target PRIVATE clogs)
target_include_directories(your_target PRIVATE clogs/include)
```

Make sure your `clogs` directory has its own `CMakeLists.txt` as shown earlier.

---

## API Usage

### Log Levels

The available log levels are defined in the `LoggingType` enum:

```c
enum LoggingType {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};
```

### Logging Function

The main logging function is:

```c
void logger(FILE *file, enum LoggingType type, const char *message);
```

- `file` – Output stream (`stdout` or file opened with `fopen`)
- `type` – Log level (e.g., `INFO`, `ERROR`)
- `message` – Log message string

However you can use the logging function directly for each log type instead:

```c
void log_debug(FILE *file, const char *message);

void log_info(FILE *file, const char *message);

void log_warning(FILE *file, const char *message);

void log_error(FILE *file, const char *message);

void log_fatal(FILE *file, const char *message);
```

---
Note: the best way to use a file for logging is to open it with the flag "a" and check this pointer.

## Code example
```c 
#include "logging.h"

int main() {
    log_init("main.log", "debug.log");

    log_info("Application initialized");
    log_debug("Checking variable values");
    
    if (some_error) {
        log_error("An error occurred during processing");
    }

    log_exit();
    return 0;
}
```
