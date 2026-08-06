#pragma once

#ifndef   RSTB_COMMON
#define   RSTB_COMMON

#ifndef RORI_DEBUG
#   define RORI_DEBUG
#endif // RORI_DEBUG

#ifndef RORI_EXIT_CODE
#   define RORI_EXIT_CODE 69
#endif // RORI_EXIT_CODE

#ifndef RORI_ASSERT
#   include <assert.h>
#   define RORI_ASSERT
#endif // RORI_ASSERT

#ifndef RORI_EXIT
#   include <stdlib.h>
#   define RORI_EXIT exit
#endif // RORI_EXIT

#ifndef RORI_ERR_PRINT
#   include <stdio.h>
#   define RORI_ERR_PRINT(...) fprintf(stderr, __VA_ARGS__)
#endif // RORI_ERR_PRINT

#define UNUSED(VAR) (void) VAR

#ifdef RORI_DEBUG
#define TODO(MSG) do { \
        RORI_ERR_PRINT("[%s:%d]: TODO : %s", __FILE__, __LINE__, MSG); \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);

#define PANIC(MSG) do { \
        RORI_ERR_PRINT("[%s:%d]: PANIC : %s", __FILE__, __LINE__, MSG); \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);

#define DBG(FMT, ...) do { \
        RORI_ERR_PRINT("[%s:%d]: " FMT, __FILE__, __LINE__, #__VA_ARGS__); \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);

#define UNIMPLEMENTED do { \
        RORI_ERR_PRINT("[%s:%d]: %s is unimplemented", __FILE__, __LINE__, __func__); \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);

#define UNREACHABLE do { \
        RORI_ERR_PRINT("[%s:%d]: UNREACHABLE", __FILE__, __LINE__, __func__); \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);
#else
#define TODO(MSG) do { \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);

#define PANIC(MSG) do { \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);

#define DBG(FMT, ...) do { \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);

#define UNIMPLEMENTED do { \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);

#define UNREACHABLE do { \
        RORI_EXIT(RORI_EXIT_CODE); \
    } while (0);
#endif

#endif    // RSTB_COMMON
