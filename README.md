# ft_safe_allocate

A memory allocation library with advanced memory tracking and safety features.

## Overview

`ft_safe_allocate` is a robust memory management library designed to enhance memory safety and debugging capabilities in C projects. It provides a unified interface for memory allocation, reallocation, and deallocation with features like automatic tracking, memory leak detection, buffer overflow protection, and thread safety.

## Key Features

- **Memory Tracking**: Automatically tracks all allocations in a hash table
- **Thread Safety**: Uses mutex locks to ensure thread-safe operations
- **Memory Fencing**: Optional guard bytes to detect buffer overflows/underflows
- **Flexible API**: Single function interface with multiple operation modes
- **Memory Leak Prevention**: Easy cleanup of all tracked allocations
- **External Memory Integration**: Track memory allocated outside the library

## Installation

### Building the Library

```bash
# Standard build
make

# Build with memory fencing enabled
make fencing

# Install to system (may require sudo)
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

## Usage

**IMPORTANT**: Once you integrate `ft_safe_allocate` into your project, you *MUST NOT* use `malloc`, `calloc`, `realloc`, or `free` directly anywhere in your code. Mixing standard allocation functions with ft_safe_allocate will break the tracking system and lead to memory leaks or corruption.

### Basic Memory Allocation

```c
// 10 elements of size int
int *array = ft_safe_allocate((size[2]){10, sizeof(int)}, ALLOCATE, NULL, NULL);

// Use the array
// ...

// Free the memory when done
array = ft_safe_allocate(NULL, FREE_ONE, array, NULL);
```
- **Note**: When you use `FREE_ONE`, make sure to assign the result of `ft_safe_allocate()` (which returns NULL) back to your variable to properly set it to **NULL**.

### Memory Reallocation

```c
size_t size[2] = {10, sizeof(int)};
int *array = ft_safe_allocate(size, ALLOCATE, NULL, NULL);

// Resize to 20 elements
size[0] = 20;
array = ft_safe_allocate(size, REALLOC, array, NULL);
```

### Tracking External Allocations

```c
void *ptr = strdup("Wiii3");
size_t size[1] = {strlen(ptr) + 1};

// Add the allocation to tracking system
ft_safe_allocate(size, ADD_TO_TRACK, ptr, NULL);

// Now it can be freed using the library
ft_safe_allocate(NULL, FREE_ONE, ptr, NULL);
```
- When dealing with other libraries that have functions which return pointers to the heap.

### Freeing Arrays of Pointers

In some cases, you allocate an array of pointers (e.g., an array of strings). You must free each pointer individually before freeing the array itself. `ft_safe_allocate()` provides a way to handle this automatically.

#### Example 1: Freeing using size

```c
size_t size[2] = {10, sizeof(char *)};
char **strings = ft_safe_allocate(size, ALLOCATE, NULL, NULL);

int i = 0;
while (i < size[0])
{
    strings[i] = ft_safe_allocate((size_t[2]){5, sizeof(char)}, ALLOCATE, NULL, NULL);
    i++;
}

// Free all strings and the array itself using the provided size
ft_safe_allocate(size, FREE_ONE, NULL, (void **)strings);
```

#### Example 2: Freeing using a NULL-terminated array

```c
size_t size[2] = {11, sizeof(char *)};
char **strings = ft_safe_allocate(size, ALLOCATE, NULL, NULL);

int i = 0;
while (i < size[0] - 1)
{
    strings[i] = ft_safe_allocate((size_t[2]){5, sizeof(char)}, ALLOCATE, NULL, NULL);
    i++;
}

// Free all strings and the array itself by detecting the NULL terminator
ft_safe_allocate(NULL, FREE_ONE, NULL, (void **)strings);
```

### Cleanup at Program End

```c
// Free all tracked allocations
ft_safe_allocate(NULL, FREE_ALL, NULL, NULL);
```

### Memory Usage Statistics

```c
unsigned int allocation_count = (unsigned int)(uintptr_t)ft_safe_allocate(NULL, GET_USAGE, NULL, NULL);
printf("Current allocations: %ui\n", allocation_count);
```

### ERRORS

**IMPORTANT**: You should always guard the return value of `ft_safe_allocate` when using it with the ALLOCATE or REALLOC actions, because if the underlying `malloc` fails, `ft_safe_allocate` will return NULL. Handling this case properly allows you to protect your program from crashing.

```c
size_t size[2] = {10, sizeof(int)};
int *array = ft_safe_allocate(size, ALLOCATE, NULL, NULL)
if (!array)
    return (ft_safe_allocate(NULL, FREE_ALL, NULL, NULL), 1);
```


## Memory Fencing

When built with memory fencing enabled, the library places guard bytes before and after each allocated memory block to detect buffer overflows and underflows.

```bash
# Build with memory fencing
make fencing
```

With fencing enabled, the library will automatically check for memory corruption when freeing memory and print detailed error messages if corruption is detected.

## Configuration

Key configuration parameters can be found in `ft_safe_allocate.h`:

- `MEMORY_FENCING`: Enable/disable guard bytes (default: false)
- `HASH_TABLE_SIZE`: Size of the allocation tracking table (default: 2048 = 2.048 kB)
- `GUARD_SIZE`: Size of guard regions in bytes (default: 8)
- `GUARD_PATTERN`: Pattern used for guard bytes (default: 0xAB)

## Thread Safety

All operations are protected by a mutex lock, making the library safe to use in multi-threaded applications.

## Error Handling

The library provides detailed warning and error messages when issues are detected:

- Memory allocation failures
- Buffer overflow/underflow detection
- Attempts to free untracked pointers
- Approaching allocation tracking limits

## Acknowledgments

This project is part of the 42 School curriculum and follows its licensing requirements.
