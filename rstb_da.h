/* 
rstb_da.h - v1.0 UnknownRori <unknownrori@proton.me>

This is a single-header-file library that provides easy to use
Dynamic Array (da) for C by using macro system.

Mainly inspired by nob.h provided by tsoding

To use this library :
#include "rstb_da.h"

Table of Contents : 
- Quick Example
- API
- Flag
- Redefinable Macros

## Quick Example

```c
#include <stdio.h>
#define RSTB_DA_H_STRIP_PREFIX
#include "rstb_da.h"

rstb_da_decl(int, MyInt);

int main()
{
    MyInt vec1 = {0};
    int data[] = { 1, 2, 3 };
    da_append(&vec1, 1);
    da_append(&vec1, 2);
    da_append(&vec1, 3);
    da_append_many(&vec1, data, 3);

    da_foreach(int, x, &vec1) {
        printf("foreach: %d\n", *x);
    }

    for (int i = 0; i < vec1.count; i++) {
        printf("for: %d\n", vec1.items[i]);
    }
    da_free(&vec1);

    return 0;
}
```

## API

 - rstb_da_decl(TYPE, NAME)         - Helper macro that declare a struct of dynamic array.
 - rstb_da_reserve(DA, COUNT)       - Helper macro that reserve memory to allow storing amount of items.
 - rstb_da_append(DA, VALUE)        - Helper macro that append value into dynamic array.
 - rstb_da_free(DA)                 - Helper macro that free dynamic array but not setting the count or capacity to 0.
 - rstb_da_reset(DA)                - Helper macro that set the count to 0 effectifly remove all the element.

## Flags

 - RSTB_DA_H_STRIP_PREFIX           - strip `rstb_` prefixes for non-redefinable names
 
## Redefinable Macros
 - RSTB_DA_ASSERT(VALUE)            - redefine which assert() of the rstb_da.h should be use.
 - RSTB_DA_REALLOC(OLDPTR, SIZE)    - redefine which realloc() of the rstb_da.h should be use.
 - RSTB_DA_FREE(PTR)                - redefine which free() of the rstb_da.h should be use.
 - RSTB_DA_INIT_CAP                 - redefine how many items should be allocated when initialized.
 
 
*/

#pragma once

#ifndef     RSTB_DA_H
#define RSTB_DA_H

#include <stddef.h>
#include <string.h>

/// Customizable macro definition

#ifndef RSTB_DA_REALLOC
    #include <stdlib.h>
    #define RSTB_DA_REALLOC realloc
#endif

#ifndef RSTB_DA_FREE
    #include <stdlib.h>
    #define RSTB_DA_FREE free
#endif

#ifndef RSTB_DA_ASSERT
    #include <assert.h>
    #define RSTB_DA_ASSERT assert
#endif

#ifndef RSTB_DA_INIT_CAP
    #define RSTB_DA_INIT_CAP 10
#endif

#define rstb_da_decl(TYPE, NAME)  \
    typedef struct {        \
        TYPE    *items;     \
        size_t  capacity;   \
        size_t  count;      \
    } NAME;

#define rstb_da_reserve(DA, EXPECTED) \
    do { \
        if ((EXPECTED) > (DA)->capacity) { \
            if ((DA)->capacity == 0) { \
                (DA)->capacity = RSTB_DA_INIT_CAP; \
            } \
            while ((EXPECTED) > (DA)->capacity) { \
                (DA)->capacity *= 2; \
            } \
            (DA)->items = RSTB_DA_REALLOC((DA)->items, (DA)->capacity * sizeof(*(DA)->items)); \
            RSTB_DA_ASSERT((DA)->items && "Buy more RAM lol"); \
        } \
    } while(0)

#define rstb_da_append(DA, VALUE) \
    do { \
        rstb_da_reserve((DA), (DA)->count + 1); \
        (DA)->items[(DA)->count++] = VALUE; \
    } while (0) 

#define rstb_da_append_many(DA, NEW_ITEM, NEW_ITEM_COUNT) \
    do { \
        rstb_da_reserve((DA), (DA)->count + (NEW_ITEM_COUNT)); \
        memcpy((DA)->items + (DA)->count, (NEW_ITEM), (NEW_ITEM_COUNT)*sizeof(*(DA)->items)); \
        (DA)->count += (NEW_ITEM_COUNT); \
    } while(0)

#define rstb_da_foreach(TYPE, VAR, DA) for (TYPE *VAR = (DA)->items; VAR < (DA)->items + (DA)->count; VAR++)

#define rstb_da_free(DA) RSTB_DA_FREE((DA)->items) 
#define rstb_da_reset(DA) (DA)->count = 0

#ifdef RSTB_DA_H_STRIP_PREFIX
    #define da_decl         rstb_da_decl
    #define da_append       rstb_da_append
    #define da_append_many  rstb_da_append_many
    #define da_foreach      rstb_da_foreach
    #define da_free         rstb_da_free
    #define da_reset        rstb_da_reset
#endif

#endif  //  RSTB_DA_H
