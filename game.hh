#pragma once

#include <stddef.h>

typedef unsigned char byte;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef int int32;

typedef size_t usize;

extern "C" {
void PlatformDrawRect(int32 x, int32 y, int32 w, int32 h, uint32 color);
void PlatformPanic(const char* file_path, int32 line, const char* message);
void PlatformLog(const char* message);

void* malloc(usize size);
void free(void* address);
}

extern "C" {
const char* GameGetTitle();
void GameInit(int screenWidth, int screenHeight);
void GameDoFrame(float deltatime, int screenWidth, int screenHeight);
void GameKeyDown(int32 key);
void GameKeyUp(int32 key);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Weverything"
#define STB_SPRINTF_STATIC
#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_NOUNALIGNED
#define STB_SPRINTF_DECORATE(name) stb_##name
#include "kerslib/thirdparty/stb_sprintf.h"
#pragma clang diagnostic pop

static char gLogbuffer[4096] = {0};
#define LogInfo(...)                                                                               \
  do {                                                                                             \
    stb_snprintf(gLogbuffer, sizeof(gLogbuffer), __VA_ARGS__);                                     \
    PlatformLog(gLogbuffer);                                                                       \
  } while (0)

#define Assert(cond, message)                                                                      \
  do {                                                                                             \
    if (cond) {                                                                                    \
      /* nothing to do */                                                                          \
    } else {                                                                                       \
      PlatformPanic(__FILE__, __LINE__, message);                                                  \
    }                                                                                              \
  } while (0)
#define Unreachable() PlatformPanic(__FILE__, __LINE__, "unreachable")

void* operator new(usize size) { return malloc(size); }
void operator delete(void* address) noexcept { free(address); }
