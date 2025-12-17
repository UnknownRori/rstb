/*
rstb_pht.h - v0.1 UnknownRori <unknownrori@proton.me>

This is a single-header-file library that provides easy to use
Shallow HashTable that only store a pointer and you need to handle your pointer allocated memory.

Table of Contents : 
- Quick Example
- API
- Flag
- Redefinable Macros
- Change Log

*/

#pragma once

#ifndef   RSTB_PHT
#define   RSTB_PHT

#include <stddef.h>
#include <string.h>

/// Customizable macro definition

#ifndef RSTB_PHT_REALLOC
    #include <stdlib.h>
    #define RSTB_PHT_REALLOC realloc
#endif

#ifndef RSTB_PHT_FREE
    #include <stdlib.h>
    #define RSTB_PHT_FREE free
#endif

#ifndef RSTB_PHT_ASSERT
    #include <assert.h>
    #define RSTB_PHT_ASSERT assert
#endif

#ifndef RSTB_PHT_INIT_CAP
    #define RSTB_PHT_INIT_CAP 10
#endif


#endif // RSTB_PHT
