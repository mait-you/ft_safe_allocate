# ft_safe_allocate

A thread-safe memory management library that provides robust allocation, tracking, and debugging capabilities to help prevent memory leaks and detect memory corruption.

## Features

- **Thread-safe memory operations** using mutex locking
- **Memory allocation tracking** to prevent memory leaks
- **Memory corruption detection** with guard bytes (when memory fencing is enabled)
- **Multiple memory management operations**:
  * Allocation with zero-initialization
  * Freeing specific pointers
  * Freeing all tracked allocations
  * Reallocation with data preservation
  * Usage statistics for tracking allocation count
- **Two implementation strategies**:
  * Array-based tracking (in `ft_safe_allocate_ARRAY/`)
  * Hash table-based tracking (in `ft_safe_allocate_HASH/`)

## Implementation Details

### Array-Based Implementation

The array-based implementation uses a fixed-size array (`MAX_ALLOCATIONS = 1024`) to store allocation metadata. This implementation is simple but has linear search complexity when freeing specific pointers.

### Hash Table-Based Implementation

The hash table-based implementation uses a hash function to place and retrieve allocations more efficiently. This improves performance for large numbers of allocations by reducing search time from O(n) to near O(1) in the average case, using open addressing for collision resolution.

## API

```c
void *ft_safe_allocate(size_t size[2], t_action action, void *ptr);
```

### Parameters

- `size`: An array of two size_t values:
  * `size[0]`: Number of elements to allocate
  * `size[1]`: Size of each element
- `action`: The operation to perform (enum `t_action`):
  * `ALLOCATE`: Allocate new memory
  * `FREE_ALL`: Free all tracked allocations
  * `FREE_ONE`: Free a specific allocation
  * `REALLOC`: Reallocate memory with data preservation
  * `GET_USAGE`: Get current allocation count
- `ptr`: Pointer for FREE_ONE and REALLOC actions

### Return Values

- For `ALLOCATE` and `REALLOC`: The newly allocated memory pointer
- For `GET_USAGE`: Number of active allocations (cast to `void*`)
- For `FREE_ONE` and `FREE_ALL`: Always returns `NULL`

## Memory Fencing

When compiled with memory fencing support (`make fencing`), the library adds guard bytes before and after each allocated memory block to detect memory corruption. If corruption is detected, the library will emit warnings to stderr.

## Building the Library

### Standard Build

```bash
make
```

### With Memory Fencing Support

```bash
make fencing
```

### Cleaning Up

```bash
make clean    # Remove object files
make fclean   # Remove object files and library
make re       # Rebuild everything
```

### Installing

```bash
sudo make install
```

This will install the library to `/usr/local/lib/` and the header file to `/usr/local/include/`.

## Usage Example

```c
#include "ft_safe_allocate.h"
#include <stdio.h>

int main(void)
{
    // Allocate an array of 10 integers
    int *arr = ft_safe_allocate((size_t[2]){10, sizeof(int)}, ALLOCATE, NULL);
    
    // Initialize array
    int i = 0;
    while (i < 10)
        arr[i] = i + 1, i++:
    
    // Print array values
    i = 0;
    while (i < 10)
        printf("%d ", arr[i]), i++;
    printf("\n");
    
    // Get current allocation count
    int count = (int)(uintptr_t)ft_safe_allocate(NULL, GET_USAGE, NULL);
    printf("Current allocations: %d\n", count);
    
    // Free the array
    ft_safe_allocate(NULL, FREE_ONE, arr);
    
    // Free all remaining allocations
    ft_safe_allocate(NULL, FREE_ALL, NULL);
    
    return (0);
}
```

## Safety Considerations

- All allocations are initialized to zero (calloc-like behavior)
- If any allocation fails, all previously allocated memory is freed
- Thread safety is ensured through mutex locking
- The library handles NULL pointers gracefully
- Tracks up to 1024 allocations (configurable via MAX_ALLOCATIONS or HASH_TABLE_SIZE)
- Warns when nearing the allocation tracking limit (>90% full)

## License

This project is part of the 42 School curriculum and follows its licensing requirements.
