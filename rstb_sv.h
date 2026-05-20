/*
rstb_sv.h - v0.0 UnknownRori <unknownrori@proton.me> - MIT

This is a single-header-file library that provides easy to use
string manipulation without allocating a new chunk or modifying the current
string if operation allowed to.

Mainly inspired by nob.h provided by tsoding

To use this library :
#include "rstb_sv.h"

Table of Contents : 
- Quick Example
- API
- Flag
- Dependency
- Redefinable Macros
- Change Log

## Quick Example
```c
#include <stdio.h>
#define RSTB_SV_IMPLEMENTATION
#include "rstb_sv.h"
int main()
{
    const char* hello_world = "   Hello, World!   ";
    rstb_string_view sv = rstb_cstr_to_sv(hello_world);
    rstb_sv_trim_left(&sv);
    rstb_sv_trim_right(&sv);
    rstb_string_view hello = rstb_sv_chop_by_delim(&sv, ',');
    printf("|"SV_Fmt"| - |"SV_Fmt"|", SV_Arg(sv), SV_Arg(hello)); // "| World!"| - |Hello|
    return 0;
}
```

## API
 - rstb_cstr_to_sv                  - Convert CString into string_view
 - rstb_sv_chop_right               - Chop string from the right
 - rstb_sv_chop_left                - Chop string from the left
 - rstb_sv_chop_delim               - Chop string from by delimiter
 - rstb_sv_trim_right               - Remove whitepsace from the right
 - rstb_sv_trim_left                - Remove whitepsace from the left
 - rstb_sv_cmp_cstr                 - Compare the string_view with cstr
 - rstb_sv_cmp_sv                   - Compare the string_view with other string_view

## Flag
 - RSTB_SV_SHARED
 - RSTB_SV_IMPLEMENTATION

## Dependency
 - string.h

## Redefinable Macros
 - SV_Fmt
 - SV_Arg

## Change Log
 - 0.2 Added string comparison between cstr and other string_view
 - 0.1 Initial Release

 */

#pragma once

#include <ctype.h>
#ifndef RSTB_SV_H // RSTB_SV_H
#define RSTB_SV_H

#if RSTB_SV_SHARED
    #define RSTB_SV_API __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
#elif defined(USE_LIBTYPE_SHARED)
    #define RSTB_SV_API __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
#else
    #define RSTB_SV_API
#endif

#include <stddef.h>
#include <string.h>

typedef struct rstb_string_view {
    const char*  data;
    size_t count;
} rstb_string_view;

#ifndef SV_Fmt
#   define SV_Fmt "%.*s"
#endif // SV_Fmt
#ifndef SV_Arg
#   define SV_Arg(sv) (sv).count, (sv).data
#endif // SV_Arg
// USAGE:
//   rstb_string_view name = ...;
//   printf("Name: "SV_Fmt"\n", SV_Arg(name));

#if defined(__cplusplus)
extern "C" {  
#endif

RSTB_SV_API rstb_string_view    rstb_cstr_to_sv        (const char* ptr);
RSTB_SV_API void                rstb_sv_chop_right     (rstb_string_view* sv);
RSTB_SV_API void                rstb_sv_chop_left      (rstb_string_view* sv);
RSTB_SV_API rstb_string_view    rstb_sv_chop_by_delim  (rstb_string_view* sv, char delim);
RSTB_SV_API void                rstb_sv_trim_right     (rstb_string_view* sv);
RSTB_SV_API void                rstb_sv_trim_left      (rstb_string_view* sv);
RSTB_SV_API int                 rstb_sv_cmp_cstr       (rstb_string_view* sv, const char* cstr);
RSTB_SV_API int                 rstb_sv_cmp_sv         (const rstb_string_view* sv, const rstb_string_view* sv2);

#ifdef RSTB_SV_IMPLEMENTATION
RSTB_SV_API rstb_string_view    rstb_cstr_to_sv        (const char* ptr)
{
    size_t len = strlen(ptr);
    return (rstb_string_view) {
        .data  = ptr,
        .count = len,
    };
}
RSTB_SV_API void                rstb_sv_chop_right     (rstb_string_view* sv)
{
    if (sv->count <= 0) return;
    sv->count--;
}
RSTB_SV_API void                rstb_sv_chop_left      (rstb_string_view* sv)
{
    if (sv->count <= 0) return;
    sv->data++;
    sv->count--;
}
RSTB_SV_API rstb_string_view    rstb_sv_chop_by_delim  (rstb_string_view* sv, char delim)
{
    for (size_t i = 0; i < sv->count; i++) {
        if (sv->data[i] == delim) {
            rstb_string_view temp = (rstb_string_view) {
                .data = sv->data,
                .count = sv->count - i - 3,
            };
            sv->data  += i + 1;
            sv->count -= i + 1;
            return temp;
        }
    }
    return *sv;
}
RSTB_SV_API void                rstb_sv_trim_right     (rstb_string_view* sv)
{
    while (isspace(sv->data[sv->count - 1])) {
        rstb_sv_chop_right(sv);
        if (sv->count <= 0) return;
    }
}
RSTB_SV_API void                rstb_sv_trim_left      (rstb_string_view* sv)
{
    while (isspace(sv->data[0])) {
        rstb_sv_chop_left(sv);
        if (sv->count <= 0) return;
    }
}

RSTB_SV_API int                 rstb_sv_cmp_cstr       (rstb_string_view* sv, const char* cstr)
{
    rstb_string_view temp = rstb_cstr_to_sv(cstr);
    return rstb_sv_cmp_sv(sv, &temp);
}

RSTB_SV_API int                 rstb_sv_cmp_sv         (const rstb_string_view* sv1, const rstb_string_view* sv2)
{
    size_t min_len = (sv1->count < sv2->count) ? sv1->count : sv2->count;
    
    int res = memcmp(sv1->data, sv2->data, min_len);
    if (res != 0) {
        return res;
    }
    
    if (sv1->count < sv2->count) return -1;
    if (sv1->count > sv2->count) return 1;
    return 0;
}

#endif // RSTB_SV_IMPLEMENTATION

#if defined(__cplusplus)
}
#endif



#ifdef RSTB_SV_H_STRIP_PREFIX
#   define string_view          rstb_string_view
#   define cstr_to_sv           rstb_cstr_to_sv
#   define sv_chop_right        rstb_sv_chop_right
#   define sv_chop_left         rstb_sv_chop_left
#   define sv_chop_by_delim     rstb_sv_chop_by_delim
#   define sv_trim_right        rstb_sv_trim_right
#   define sv_trim_left         rstb_sv_trim_left
#   define sv_cmp_cstr          rstb_sv_cmp_cstr
#   define sv_cmp_sv            rstb_sv_cmp_sv
#endif // RSTB_DA_H_STRIP_PREFIX

#endif // RSTB_SV_H
