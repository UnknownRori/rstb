/* 
rstb_arena.h - v0.1 UnknownRori <unknownrori@proton.me>

This is a single-header-file library that provides easy to use
Arena (Bump) allocator for C by using macro system., by default it uses malloc as backend.
Note that by default it will be aligned by 16 bytes

Supported backend
- malloc
- windows.h (Windows API)

To use this library :
#define RSTB_ARENA_IMPLEMENTATION
#include "rstb_arena.h"

Table of Contents : 
- Quick Example
- API
- Flag
- Redefinable Macros
- Change Log

## Quick Example

```c
#include <stdio.h>
#include <string.h>

#define RSTB_ARENA_IMPLEMENTATION
#define RSTB_ARENA_STRICT
#define RSTB_ARENA_BACKEND_MALLOC
#include "rstb_arena.h"

int main()
{
    rstb_arena arena = {0};
    rstb_arena_init(&arena, 32);

    char* hello = rstb_arena_alloc(&arena, 8);
    strncpy(hello, "Hello!", 8);
    printf("hello = %p\n", hello);
    printf("%s\n", hello);

    char* hello2 = rstb_arena_alloc(&arena, 32);
    strncpy(hello2, "Hello World!", 32);
    printf("hello = %p\n", hello2);
    printf("%s\n", hello2);

    rstb_arena_free(&arena);

    return 0;
}
```

## API

## Functions
 - rstb_arena_init                      - Initialize [`arena`] with region size and allocate it's first region based on allocation backend
 - rstb_arena_free                      - Free all of the region from [`arena`]
 - rstb_arena_reset                     - Mark all region as free section
 - rstb_arena_alloc                     - Request a memory slot
 - rstb_arena_realloc                   - Reallocate old memory to new memory probably a new region but the old still exists and going to be memory leak (which is safe btw)

## Constants

 - RSTB_ARENA_RECOMMENDED               - Recommended size for arena

## Flags

 - RSTB_ARENA_STRIP_PREFIX              - strip `rstb_` prefixes for non-redefinable names
 - RSTB_ARENA_STRICT                    - when memory allocation failed throw an assertion error immediately 
 
 - RSTB_ARENA_BACKEND_MALLOC            - Uses C stdlib memory management
 - RSTB_ARENA_BACKEND_WINDOWS           - Uses windows memory management
 - RSTB_ARENA_BACKEND_WINDOWS           - Uses mmap memory management
## Redefinable Macros

 - RSTB_ARENA_ASSERT(VALUE)            - redefine which assert() of the rstb_da.h should be use.
 - RSTB_ARENA_REALLOC(OLDPTR, SIZE)    - redefine which realloc() of the rstb_da.h should be use.
 - RSTB_ARENA_FREE(PTR)                - redefine which free() of the rstb_da.h should be use.
 - RSTB_ARENA_ALIGN                    - redefine how memory alignment should be
 
## Change Log
 - 0.1  - Add rstb_arena_reset, realloc, add extern c for cpp, and more docs
 - 0.0  - Proof of concept
 
*/


#pragma once

#ifndef   RSTB_ARENA
#define   RSTB_ARENA

#include <stddef.h>
#include <string.h>

#ifndef RSTB_ARENA_ASSERT
    #include <assert.h>
    #define RSTB_ARENA_ASSERT assert
#endif // RSTB_ARENA_ASSERT
//
#ifndef RSTB_ARENA_ALIGN
    #define RSTB_ARENA_ALIGN 16
#endif // RSTB_ARENA_ALIGN

#define _RSTB_ARENA_ALIGN_UP(value, alignment) \
    ((((value) + (alignment) - 1) / (alignment)) * (alignment))

#ifndef RSTB_ARENA_RECOMMENDED
    #define RSTB_ARENA_RECOMMENDED 4096
#endif // RSTB_ARENA_RECOMMENDED

#ifdef RSTB_ARENA_STRICT
    #define RSTB_ARENA_STRICT_ASSERT RSTB_ARENA_ASSERT
#else
    #define RSTB_ARENA_STRICT_ASSERT(X) (X)
#endif // RSTB_ARENA_STRICT

#ifdef RSTB_ARENA_BACKEND_MALLOC
    #ifndef RSTB_ARENA_REALLOC
        #include <stdlib.h>
        #define RSTB_ARENA_REALLOC realloc
    #endif

    #ifndef RSTB_ARENA_FREE
        #include <stdlib.h>
        #define RSTB_ARENA_FREE free
    #endif
#elifdef RSTB_ARENA_BACKEND_WINDOWS
    #ifndef RSTB_ARENA_REALLOC
        #include <windows.h>
        #define RSTB_ARENA_REALLOC(P, S) VirtualAlloc((P), (S), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE )
    #endif

    #ifndef RSTB_ARENA_FREE
        #include <windows.h>
        #define RSTB_ARENA_FREE(X) VirtualFree((X), 0, MEM_DECOMMIT)
    #endif
#elif RSTB_ARENA_BACKEND_MMAP
    #error "Currently not supported!"
#elif RSTB_ARENA_BACKEND_EMSCRIPTEN
    #error "Currently not supported!"
#else
    #ifndef RSTB_ARENA_REALLOC
        #include <stdlib.h>
        #define RSTB_ARENA_REALLOC realloc
    #endif

    #ifndef RSTB_ARENA_FREE
        #include <stdlib.h>
        #define RSTB_ARENA_FREE free
    #endif
#endif

#if RSTB_ARENA_SHARED
    #define RSTB_ARENA_API __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
#elif defined(USE_LIBTYPE_SHARED)
    #define RSTB_ARENA_API __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
#else
    #define RSTB_ARENA_API
#endif

// ---------------------------------------

typedef struct rstb_arena_region {
    char*   ptr;
    size_t  capacity;
    size_t  used;

    struct rstb_arena_region* next;
} rstb_arena_region;

typedef struct rstb_arena {
    rstb_arena_region* next;
    size_t             region_size;
} rstb_arena;

#if defined(__cplusplus)
extern "C" {  
#endif

// Initialize [`arena`] with region size and allocate it's first region based on allocation backend
RSTB_ARENA_API int         rstb_arena_init(rstb_arena* arena, size_t region_size);
// Request a memory slot
RSTB_ARENA_API void*       rstb_arena_alloc(rstb_arena* arena, size_t size);
// Reallocate old memory to new memory probably a new region but the old still exists and going to be memory leak (which is safe btw)
RSTB_ARENA_API void*       rstb_arena_realloc(rstb_arena* arena, void* oldptr, size_t old, size_t new_size);
// Mark all region as free section
RSTB_ARENA_API void        rstb_arena_reset(rstb_arena* arena);
// Free all of the region from [`arena`]
RSTB_ARENA_API void        rstb_arena_free(rstb_arena* ptr);

#if defined(__cplusplus)
}
#endif


#ifdef RSTB_ARENA_IMPLEMENTATION


// INFO : Private functions
static int                __rstb_arena_append_region(rstb_arena* arena, size_t n);
static rstb_arena_region* __rstb_arena_alloc_region(size_t capacity);
static void               __rstb_arena_free_region(rstb_arena_region* ptr);

int         rstb_arena_init(rstb_arena* ptr, size_t region_size)
{
    RSTB_ARENA_ASSERT(ptr != NULL && "Null Pointer!");
    RSTB_ARENA_ASSERT(region_size > 0 && "Size cannot be 0!");

    ptr->region_size = _RSTB_ARENA_ALIGN_UP(region_size, RSTB_ARENA_ALIGN);

    if (__rstb_arena_append_region(ptr, ptr->region_size)) {
        RSTB_ARENA_STRICT_ASSERT(0 && "Buy more RAM lol");
        return 1;
    }


    return 0;
}

void*       rstb_arena_alloc(rstb_arena* arena, size_t size)
{
    size_t aligned = _RSTB_ARENA_ALIGN_UP(size, RSTB_ARENA_ALIGN);
    rstb_arena_region* next = arena->next;
    while (1) {
        if (next->capacity > next->used + aligned) {
            break;
        }

        if (next->next == NULL)  {
            size_t new_size = arena->region_size;
            if (size > new_size) {
                new_size = size;
            }
            __rstb_arena_append_region(arena, size);
            next = next->next;
            goto done;
        }

        next = next->next;
    }

done:
    void* ptr = next->ptr + next->used;
    next->used += aligned;

    return ptr;
}

RSTB_ARENA_API void*       rstb_arena_realloc(rstb_arena* arena, void* oldptr, size_t old, size_t new_size)
{
    RSTB_ARENA_ASSERT(arena != NULL && "Null ptr: arena");
    RSTB_ARENA_ASSERT(oldptr != NULL && "Null ptr: oldptr");
    RSTB_ARENA_ASSERT(old > 0 && "old must above 0");
    RSTB_ARENA_ASSERT(new_size > 0 && "new_size must above 0");
    void* tmp = rstb_arena_alloc(arena, new_size);
    memcpy(tmp, oldptr, old);
    return tmp;
}

void        rstb_arena_free(rstb_arena* ptr)
{
    RSTB_ARENA_ASSERT(ptr != NULL && "Null Pointer!");

    rstb_arena_region* next = ptr->next;
    while (next != NULL) {
        rstb_arena_region* temp = next->next;
        RSTB_ARENA_FREE(next);
        next = temp;
    }

    ptr->region_size = 0;
    ptr->next = NULL;
}

RSTB_ARENA_API void        rstb_arena_reset(rstb_arena* arena)
{
    rstb_arena_region* next = arena->next;
    while (1) {
        next->used = 0;

        if (next->next == NULL) break;
        next = next->next;
    }
}


static int                __rstb_arena_append_region(rstb_arena* arena, size_t n)
{
    if (arena->next == NULL) {
        arena->next = __rstb_arena_alloc_region(arena->region_size);
        return 0;
    }

    rstb_arena_region* next = arena->next;
    while (1) {
        if (next->next == NULL) break;
        next = next->next;
    }
    next->next = __rstb_arena_alloc_region(arena->region_size + sizeof(rstb_arena_region));
    return 0;
}

static rstb_arena_region* __rstb_arena_alloc_region(size_t capacity)
{
    rstb_arena_region* ptr = NULL;
    ptr = (rstb_arena_region*)RSTB_ARENA_REALLOC(ptr, capacity);
    if (ptr == NULL) {
        RSTB_ARENA_STRICT_ASSERT(0 && "Buy more RAM lol");
        return NULL;
    }
    ptr->next = NULL;
    ptr->used = 0;
    ptr->ptr = ((char*)ptr) + sizeof(rstb_arena_region);
    ptr->capacity = capacity - sizeof(rstb_arena_region);
    return ptr;
}

static void               __rstb_arena_free_region(rstb_arena_region* ptr)
{
    RSTB_ARENA_FREE(ptr);
}

#endif // RSTB_ARENA_IMPLEMENTATION

#ifdef RSTB_ARENA_STRIP_PREFIX
    #define arena                   rstb_arena
    #define arena_init              rstb_arena_init
    #define arena_alloc             rstb_arena_alloc
    #define arena_realloc           rstb_arena_realloc
    #define arena_reset             rstb_arena_reset
    #define arena_free              rstb_arena_free
#endif // RSTB_ARENA_STRIP_PREFIX

#endif // RSTB_ARENA
