#pragma once

typedef unsigned char byte;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef int int32;

void PlatformDrawRect(int32 x, int32 y, int32 w, int32 h, uint32 color);
void PlatformPanic(const char* file_path, int32 line, const char* message);
void PlatformLog(const char* message);

void GameInit(uint32 width, uint32 height);
void GameScreenResize(uint32 width, uint32 height);
void GameDoFrame(float deltatime);
void GameKeyDown(int32 key);
void GameKeyUp(int32 key);

#define STB_SPRINTF_STATIC
#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_NOUNALIGNED
#define STB_SPRINTF_DECORATE(name) stb_##name
#include "kerslib/thirdparty/stb_sprintf.h"

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