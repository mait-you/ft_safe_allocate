# ft_safe_allocate

A robust memory management library with advanced tracking and safety features for C projects.

<div align="center">

![Memory Safety](https://img.shields.io/badge/Memory-Safety-brightgreen)
![Thread Safe](https://img.shields.io/badge/Thread-Safe-blue)
![License](https://img.shields.io/badge/License-42-lightgrey)

</div>

## 🔎 Overview

`ft_safe_allocate` is a comprehensive memory management solution designed to enhance memory safety and provide debugging capabilities in C projects. It offers a unified interface for memory operations while providing critical features like automatic tracking, leak detection, buffer overflow protection, and thread safety.

## ✨ Key Features

- **🔄 Unified Memory Interface**: Single function handles all memory operations
- **📊 Automatic Memory Tracking**: All allocations are tracked in a hash table
- **🔒 Thread Safety**: Built-in mutex locks ensure thread-safe operations
- **🛡️ Memory Fencing**: Optional guard bytes detect buffer overflows/underflows
- **🔍 Leak Detection**: Easy identification and cleanup of all tracked allocations
- **🔌 External Memory Integration**: Track memory allocated outside the library
- **📝 Detailed Error Reporting**: Clear messages for memory issues and corruption

## 📦 Installation

### Building the Library

```bash
# Standard build
make

# Build with memory fencing enabled
make fencing

# Install to system
make install

# Clean up object files
make clean

# Remove all generated files
make fclean

# Rebuild from scratch
make re
```

### Including in Your Project

```c
#include "ft_safe_allocate.h"

// Link with -lft_safe_allocate
```

## 🚀 Usage

### ⚠️ Important Note

Once you integrate `ft_safe_allocate` into your project, you **MUST NOT** use standard allocation functions (`malloc`, `calloc`, `realloc`, or `free`) directly anywhere in your code. Mixing standard functions with `ft_safe_allocate` will break the tracking mechanism and lead to memory leaks or corruption.

### Basic Memory Allocation

```c
// Allocate an array of 10 integers
int *array = ft_safe_allocate((size_t[2]){10, sizeof(int)}, ALLOCATE, NULL, NULL);

// Use the array
// ...

// Free the memory when done
array = ft_safe_allocate(NULL, FREE_ONE, array, NULL);
```

> **Note**: When using `FREE_ONE`, assign the result back to your variable to properly set it to `NULL`.

### Memory Reallocation

```c
size_t size[2] = {10, sizeof(int)};
int *array = ft_safe_allocate(size, ALLOCATE, NULL, NULL);

// Resize to 20 elements
size[0] = 20;
array = ft_safe_allocate(size, REALLOC, array, NULL);
```

### Tracking External Allocations

When working with libraries that return heap pointers:

```c
void *ptr = strdup("Hello World");
size_t size[1] = {strlen(ptr) + 1};

// Add the allocation to tracking system
ft_safe_allocate(size, ADD_TO_TRACK, ptr, NULL);

// Now it can be freed using the library
ft_safe_allocate(NULL, FREE_ONE, ptr, NULL);
```

### Freeing Arrays of Pointers

#### Freeing using size information:

```c
// Allocate array of string pointers
size_t size[2] = {10, sizeof(char *)};
char **strings = ft_safe_allocate(size, ALLOCATE, NULL, NULL);

// Allocate individual strings
while (i < size[0])
{
    strings[i] = ft_safe_allocate((size_t[2]){5, sizeof(char)}, ALLOCATE, NULL, NULL);
    i++:
}

// Free all strings and the array itself using the provided size
ft_safe_allocate(size, FREE_ONE, NULL, (void **)strings);
```

#### Freeing NULL-terminated arrays:

```c
// Allocate array (with space for NULL terminator)
size_t size[2] = {11, sizeof(char *)};
char **strings = ft_safe_allocate(size, ALLOCATE, NULL, NULL);

// Allocate individual strings (leaving last element NULL)
int i = 0;
while (i < size[0] - 1)
{
    strings[i] = ft_safe_allocate((size_t[2]){5, sizeof(char)}, ALLOCATE, NULL, NULL);
    i++:
}

// Free all strings and the array itself (detects NULL terminator)
ft_safe_allocate(NULL, FREE_ONE, NULL, (void **)strings);
```

### Cleanup at Program End

```c
// Free all tracked allocations
ft_safe_allocate(NULL, FREE_ALL, NULL, NULL);
```

### Memory Usage Statistics

```c
unsigned int count = (unsigned int)(uintptr_t)ft_safe_allocate(NULL, GET_USAGE, NULL, NULL);
printf("Current allocations: %u\n", count);
```

## ⚠️ Error Handling

Always check return values to handle allocation failures:

```c
size_t size[2] = {10, sizeof(int)};
int *array = ft_safe_allocate(size, ALLOCATE, NULL, NULL);
if (!array) {
    // Clean up and exit
    ft_safe_allocate(NULL, FREE_ALL, NULL, NULL);
    return 1;
}
```

## 🛡️ Memory Fencing

When built with memory fencing enabled, the library places guard bytes before and after each allocated block to detect buffer corruption:

```bash
# Build with memory fencing
make fencing
```

The library automatically checks for memory corruption when freeing memory and prints detailed error messages if detected.

## ⚙️ Configuration

Key configuration parameters can be found in `ft_safe_allocate.h`:

<div align="center">

| Parameter | Description | Default |
|-----------|-------------|---------|
| `MEMORY_FENCING` | Enable/disable guard bytes | `false` |
| `HASH_TABLE_SIZE` | Size of allocation tracking table | `2048` |
| `GUARD_SIZE` | Size of guard regions in bytes | `8` |
| `GUARD_PATTERN` | Pattern for guard bytes | `0xAB` |

</div>

## 🧵 Thread Safety

All operations are protected by a mutex lock, making the library safe for multi-threaded applications.

## 📊 Error Messages

The library provides detailed warnings and errors:

- Memory allocation failures
- Buffer overflow/underflow detection
- Attempts to free untracked pointers
- Approaching allocation tracking limits

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📜 License

This project is part of the 42 School curriculum and follows its licensing requirements.
