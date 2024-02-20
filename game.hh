#pragma once

#include "kerslib/core.hh"

extern "C" {
const char* GameGetTitle();
void GameInit(int screenWidth, int screenHeight);
void GameDoFrame(float deltatime, int screenWidth, int screenHeight);
void GameKeyDown(int32 key);
void GameKeyUp(int32 key);
}