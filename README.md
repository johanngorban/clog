# Logging library for C

A lightweight logging library for C projects, supporting multiple log levels and output to `stdout` or file, built in pure C and CMake

---
# How to use
clog is built using pure C and CMake, so it does not require any other dependencies. All you need is in the list below:
- CMake (starting with 3.16)
- gcc or clang (c11)
---

# API reference
You can use direct or mediaded logging. Direct logging allows you specify what type of logging you want to use (the list below), message you want to see in the log, log source file and line. Instead of that you can use mediaded (e.g. simplified) functions.


### Log types
The log type you need to specify to make a log note. There is no point to describe every type, I hope these names are clear enough
```c
typedef enum {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
} LoggingType;
```

### Functions

```c
int log_file_append(const char *path)
```

You can `log_file_append` if you need add one more log file. It requires the file path. Returns `1` if file initialized successfully and `0` otherwise.

```c
void logger(LoggingType type, const char *message)
```

`logger` is the main function for logging. It needs only log type and message you want to display. It will display the message in all log files you initialized before. 

Note: if you want more control and need to specify source file and line, you can use `logger_` instead. See source code for that


```c
void log_debug(message_)

void log_info(message_)

void log_warning(message_)

void log_error(message_)

void log_fatal(message_)
```

Here is a bunch of functions for simplified logging. There is no need to describe every function separately because of the names. Every function accepts message and displays it in all log files initialized before

Note: actually, these functions are macros. So you do not have to worry about stack usage. It is just the interface of `logger_` function

```c
void log_exit()
```

The function for deinitialization of all log files. It closes files and deallocates file descriptors. This function is *optional* since it is set as `atexit` in the initialization function `log_init`.

---