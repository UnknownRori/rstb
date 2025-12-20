/* 
rstb_arena.h - v0.0 UnknownRori <unknownrori@proton.me>

This is a single-header-file library that provides easy to use
Arena (Bump) allocator for C by using macro system., by default it uses malloc as backend.

Mainly inspired by nob.h provided by tsoding

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
#define RSTB_ARENA_IMPLEMENTATION
#define RSTB_ARENA_STRIP_PREFIX
#include "rstb_da.h"

int main()
{
    return 0;
}
```

## API

## Flags

 - RSTB_ARENA_STRIP_PREFIX           - strip `rstb_` prefixes for non-redefinable names
 
## Redefinable Macros

 - RSTB_ARENA_ASSERT(VALUE)            - redefine which assert() of the rstb_da.h should be use.
 - RSTB_ARENA_REALLOC(OLDPTR, SIZE)    - redefine which realloc() of the rstb_da.h should be use.
 - RSTB_ARENA_FREE(PTR)                - redefine which free() of the rstb_da.h should be use.
 - RSTB_ARENA_INIT_CAP                 - redefine how many items should be allocated when initialized.
 
## Change Log
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
#endif

#ifndef RSTB_ARENA_INIT_CAP
    #define RSTB_ARENA_INIT_CAP 4096
#endif

#ifdef RSTB_ARENA_STRICT
    #define RSTB_ARENA_STRICT_ASSERT RSTB_ARENA_ASSERT
#else
    #define RSTB_ARENA_STRICT_ASSERT(X) (X)
#endif


#ifdef RSTB_ARENA_BACKEND_MALLOC
    #ifndef RSTB_ARENA_REALLOC
        #include <stdlib.h>
        #define RSTB_ARENA_REALLOC realloc
    #endif

    #ifndef RSTB_ARENA_FREE
        #include <stdlib.h>
        #define RSTB_ARENA_FREE free
    #endif
#elif RSTB_ARENA_BACKEND_WINDOWS
    #error "Currently not supported!"
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
    void*   ptr;
    size_t  capacity;
    size_t  used;

    rstb_arena_region* next;
} rstb_arena_region;

typedef struct rstb_arena {
    rstb_arena_region* next;
    size_t             region_size;
} rstb_arena;

RSTB_ARENA_API int         rstb_arena_init(rstb_arena* arena, size_t region_size);
RSTB_ARENA_API void*       rstb_arena_alloc(size_t size);
RSTB_ARENA_API void        rstb_arena_free(rstb_arena* ptr);

// INFO : Private functions
int                __rstb_arena_append_region(rstb_arena* arena, size_t capacity);
rstb_arena_region* __rstb_arena_alloc_region(size_t capacity);
void               __rstb_arena_free_region(rstb_arena_region* ptr);


/*#ifdef RSTB_ARENA_IMPLEMENTATION*/

int         rstb_arena_init(rstb_arena* ptr, size_t region_size)
{
    RSTB_ARENA_ASSERT(ptr != NULL && "Null Pointer!");
    RSTB_ARENA_ASSERT(region_size <= 0 && "<= 0 Region Size is not allowed!");

    ptr->region_size = region_size;
    if (__rstb_arena_append_region(ptr, region_size)) {
        RSTB_ARENA_STRICT_ASSERT(0 && "Buy more RAM lol");
        return 1;
    }

    return 0;
}

void*       rstb_arena_alloc(size_t size)
{
    return NULL;
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

// INFO : Private functions
int                __rstb_arena_append_region(rstb_arena* arena, size_t capacity);
rstb_arena_region* __rstb_arena_alloc_region(size_t capacity);
void               __rstb_arena_free_region(rstb_arena_region* ptr);

/*#endif*/

#ifdef RSTB_ARENA_STRIP_PREFIX
    #define arena                   rstb_arena
    #define arena_init              rstb_arena_init
    #define arena_alloc             rstb_arena_alloc
    #define arena_free              rstb_arena_free
#endif // RSTB_ARENA_STRIP_PREFIX

#endif // RSTB_ARENA
