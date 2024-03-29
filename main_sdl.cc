#include "game.cc"

#include "kerslib/core.hh"

#include <stdio.h>
#include <stdlib.h>

#include "kerslib/thirdparty/SDL2/SDL.h"

static SDL_Window* gWindow     = nullptr;
static SDL_Renderer* gRenderer = nullptr;

void PlatformLog(const char* file, int line, const char* message) {
  printf("%s -- %s:%d\n", message, file, line);
}
void PlatformPanic(const char* file, int line, const char* message) {
  fprintf(stderr, "PANIC: %s -- %s:%d\n", message, file, line);
  abort();
}
void PlatformDrawRect(int x, int y, int w, int h, uint32 color) {
  AssertM(gRenderer, "No renderer");
  SDL_Rect rect = {
      x,
      y,
      w,
      h,
  };
  Uint8 r = (color >> 24) & 0xFF;
  Uint8 g = (color >> 16) & 0xFF;
  Uint8 b = (color >> 8) & 0xFF;
  Uint8 a = (color >> 0) & 0xFF;
  SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
  SDL_RenderFillRect(gRenderer, &rect);
}

int main(int argc, char* argv[]) {
  // TODO
  (void)argc;
  (void)argv;

  int windowPosX   = SDL_WINDOWPOS_CENTERED_DISPLAY(1);
  int windowPosY   = SDL_WINDOWPOS_CENTERED_DISPLAY(1);
  int screenWidth  = 640;
  int screenHeight = 360;

  const char* windowTitle = GameGetTitle();

  SDL_Init(SDL_INIT_VIDEO);
  gWindow = SDL_CreateWindow(
      windowTitle,
      windowPosX,
      windowPosY,
      screenWidth,
      screenHeight,
      SDL_WINDOW_RESIZABLE
  );
  GameInit(screenWidth, screenHeight);

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

  bool isRunning = true;
  while (isRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT: {
        isRunning = false;
      } break;
      case SDL_KEYDOWN: {
        if (event.key.keysym.sym == SDLK_ESCAPE)
          isRunning = false;
        GameKeyDown(event.key.keysym.sym);
      } break;
      case SDL_KEYUP: {
        GameKeyUp(event.key.keysym.sym);
      } break;
      case SDL_WINDOWEVENT: {
        switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: {
          screenWidth  = event.window.data1;
          screenHeight = event.window.data2;
        } break;
        }
      } break;
      }
    }

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);
    GameDoFrame(1.0f / 60.0f, screenWidth, screenHeight);
    SDL_RenderPresent(gRenderer);
  }

  SDL_Quit();

  return 0;
}
