# Logging library for C

A lightweight logging library for C projects, supporting multiple log levels and output to `stdout` or file, built in pure C and CMake

---
# How to use
loggy is built using pure C and CMake, so it does not require any other dependencies. All you need is in the list below:
- CMake (starting with 3.16)
- gcc or clang (c11)
---

# How to build
Follow the steps below to build loggy

```bash
# build shared (so/dll) library
cmake -B build -DBUILD_SHARED_LIB=ON
cd build
# install library into the system
make install
```

> Note: sometimes you need to run `make install` as sudo.

Now you can use loggy. Here is CMake instructions you need to use loggy

```cmake
find_package(loggy REQUIRED)

target_link_libraries(YOUR_PROGRAM
    PUBLIC
        loggy
)
```

### Cannot open shared object file on Linux
If you get a message: `error while loading shared libraries: libloggy.so: cannot open shared object file: No such file or directory`, you need to run the following commands:
```bash
echo "/usr/local/lib64" | sudo tee /etc/ld.so.conf.d/loggy.conf
sudo ldconfig
```

---

# API reference
You can use direct or mediaded logging. Direct logging allows you specify what type of logging you want to use (the list below), message you want to see in the log, log source file and line. Instead of that you can use mediaded (e.g. simplified) functions.

## Log flags
There are a several log flags you can use to configure your logs
```
#define LOG_DATE 0 // Add date dd/mm/yy
#define LOG_TIME 1 // Add time hh/mm/ss
#define LOG_PATH 2 // Add full path of log source
#define LOG_LINE 4 // Add line number

// Shortcuts
#define LOG_SHORT   LOG_DATE | LOG_TIME
#define LOG_FULL    LOG_DATE | LOG_TIME | LOG_PATH | LOG_LINE
```

Using thems, you can specify information you want to log. As you can see, log type and message are still required


### Log types
The log type you need to specify to make a log note. There is no point to describe every type, I hope these names are clear enough
```c
typedef enum {
    FATAL   = 0,
    ERROR   = 1,
    INFO    = 2,
    WARNING = 3,
    DEBUG   = 4,
} log_level_t;
```

### Functions

```c
void set_loggy_flags(unsigned int flags)
```
Set log flags, see "Log flags" section above for details

```c
int log_stdout_append()
```
Call it, if you need to log into stdout

```c
int log_file_append(const char *path)
```

You can `log_file_append` if you need add one more log file. It requires the file path. Returns `1` if file initialized successfully and `0` otherwise.

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
