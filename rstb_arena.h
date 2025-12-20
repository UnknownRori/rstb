/* 
rstb_arena.h - v0.0 UnknownRori <unknownrori@proton.me>

This is a single-header-file library that provides easy to use
Arena (Bump) allocator for C by using macro system.

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

#ifndef RSTB_ARENA_REALLOC
    #include <stdlib.h>
    #define RSTB_ARENA_REALLOC realloc
#endif

#ifndef RSTB_ARENA_FREE
    #include <stdlib.h>
    #define RSTB_ARENA_FREE free
#endif

#ifndef RSTB_ARENA_ASSERT
    #include <assert.h>
    #define RSTB_ARENA_ASSERT assert
#endif

#ifndef RSTB_ARENA_INIT_CAP
    #define RSTB_ARENA_INIT_CAP 4096
#endif

typedef struct rstb_arena {

} rstb_arena;

/*#ifdef RSTB_ARENA_IMPLEMENTATION*/


/*#endif*/

#ifdef RSTB_ARENA_STRIP_PREFIX
    #define arena                 rstb_arena
#endif // RSTB_ARENA_STRIP_PREFIX

#endif // RSTB_ARENA
