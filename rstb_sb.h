// rstb_sv.h - v0.0 UnknownRori <unknownrori@proton.me> - MIT
// 
// This is a single-header-file library that provides easy to use
// String builder, uses my <rstb_da.h>
#if 0
#include "rstb_da.h"
#define RSTB_SB_IMPLEMENTATION
#include "rstb_sb.h"
#include <stdio.h>

int main()
{
    rstb_sb sb = {0};
    rstb_sb_reserve(&sb, 64);
    rstb_sb_append(&sb, "Hello: ");
    rstb_sb_appendf(&sb, "%d\n", 69);
    printf("%s", sb.items);
    return 0;
}
#endif
#pragma once

#ifndef RSTB_SB_H
#ifndef RSTB_DA_H
    #error "Require rstb_da.h"
#endif

#if RSTB_SB_SHARED
    #define RSTB_SB_API __declspec(dllexport)     // We are building the library as a Win32 shared library (.dll)
#elif defined(USE_LIBTYPE_SHARED)
    #define RSTB_SB_API __declspec(dllimport)     // We are using the library as a Win32 shared library (.dll)
#else
    #define RSTB_SB_API
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

rstb_da_decl(char, rstb_sb);


#if defined(__cplusplus)
extern "C" {  
#endif

RSTB_SB_API void rstb_sb_reserve(rstb_sb* self, size_t reserve);
RSTB_SB_API void rstb_sb_reset(rstb_sb* self);
RSTB_SB_API void rstb_sb_free(rstb_sb* self);
RSTB_SB_API int rstb_sb_append(rstb_sb* self, const char* str);
RSTB_SB_API int rstb_sb_appendf(rstb_sb* self, const char* format, ...);
RSTB_SB_API int rstb_sb_vsappendf(rstb_sb* self, const char* format, va_list args);

#if defined(__cplusplus)
}
#endif // __cplusplus

#ifdef RSTB_SB_IMPLEMENTATION
// #if 1
RSTB_SB_API void rstb_sb_reserve(rstb_sb* self, size_t reserve) { rstb_da_reserve(self, reserve); }
RSTB_SB_API void rstb_sb_reset(rstb_sb* self) { rstb_sb_reset(self); }
RSTB_SB_API void rstb_sb_free(rstb_sb* self) { rstb_sb_free(self); }
RSTB_SB_API int rstb_sb_append(rstb_sb* self, const char* str)
{
    size_t len = strlen(str);
    rstb_da_append_many(self, str, len);
    return len;
}
RSTB_SB_API int rstb_sb_vsappendf(rstb_sb* self, const char* format, va_list args)
{
    va_list copy;
    va_copy(copy, args);
    int needed = vsnprintf(NULL, 0, format, copy);
    va_end(copy);
    
    if (needed < 0)
        return 0;
    
    size_t required = self->count + (size_t)needed + 1;
    rstb_da_reserve(self, required);
    
    vsnprintf(
        self->items + self->count,
        self->capacity - self->count,
        format,
        args);
    
    self->count += (size_t)needed;

    return needed;
}
RSTB_SB_API int rstb_sb_appendf(rstb_sb* self, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int needed = rstb_sb_vsappendf(self, format, args);
    va_end(args);

    return needed;
}
#endif

#ifdef RSTB_SB_STRIP_PREFIX
#define sb_init(...)   rstb_sb_init(__VA_ARGS__)
#define sb_reserve(...)   rstb_sb_reserve(__VA_ARGS__)
#define sb_free(...)   rstb_sb_free(__VA_ARGS__)
#define sb_append(...)     rstb_sb_append(__VA_ARGS__)
#define sb_appendf(...)     rstb_sb_appendf(__VA_ARGS__)
#define sb_vsappendf(...)   rstb_sb_vsappendf(__VA_ARGS__)
#endif

#endif //  RSTB_SB_H
