#include "game.hh"

typedef uint32 Color;

static const Color BLACK   = 0x000000FF;
static const Color WHITE   = 0xFFFFFFFF;
static const Color RED     = 0xFF0000FF;
static const Color GREEN   = 0x00FF00FF;
static const Color BLUE    = 0x0000FFFF;
static const Color MAGENTA = RED | BLUE;
static const Color YELLOW  = RED | GREEN;
static const Color CYAN    = BLUE | GREEN;

static const Color COLOR_LIST[] = {BLACK, WHITE, RED, GREEN, BLUE, MAGENTA, YELLOW, CYAN};

struct Input {
  float deltatime = 1.0f / 60.0f;

  bool keydownA     = false;
  bool keydownAPrev = false;
  bool keydownS     = false;
  bool keydownSPrev = false;
  bool keydownD     = false;
  bool keydownDPrev = false;

  int screenWidth        = 100;
  int screenHeight       = 100;
  bool screenSizeChanged = true;

  Input(uint32 screenWidth, uint32 screenHeight)
      : screenWidth(screenWidth)
      , screenHeight(screenHeight)
      , screenSizeChanged(true) {}

  void OnScreenResize(uint32 width, uint32 height) {
    if (screenWidth != width || screenHeight != height) {
      screenSizeChanged = true;
      screenWidth       = width;
      screenHeight      = height;
    }
  }

  void OnKeyDown(int32 key) {
    if (key == 'a')
      keydownA = true;
    if (key == 's')
      keydownS = true;
    if (key == 'd')
      keydownD = true;
  }

  void OnKeyUp(int32 key) {
    if (key == 'a')
      keydownA = false;
    if (key == 's')
      keydownS = false;
    if (key == 'd')
      keydownD = false;
  }

  void BeginFrame(float deltatime) { this->deltatime = deltatime; }
  void EndFrame() {
    keydownAPrev = keydownA;
    keydownSPrev = keydownS;
    keydownDPrev = keydownD;
  }
};

struct GameState {

  int colorIndex   = 0;
  float bouncyPosX = 0;
  float bouncyPosY = 0;
  float bouncyVelX = 0;
  float bouncyVelY = 0;
  int bouncySize   = 0;

  void SerializeFrom(const char* filepath) {}
  void SerializeTo(const char* filepath) {}

  void OnKeyDown(int32 key) {}
  void OnKeyUp(int32 key) {}

  GameState(Input& input) {
    bouncyPosX = input.screenWidth / 2.0f;
    bouncyPosY = input.screenHeight / 2.0f;
    bouncyVelX = input.screenWidth / 2.0f;
    bouncyVelY = input.screenWidth / 2.0f;
    bouncySize = input.screenHeight / 10;
  }

  void DoFrame(Input& input) {
    PlatformDrawRect(0, 0, input.screenWidth, input.screenHeight, COLOR_LIST[colorIndex]);

    if (input.screenSizeChanged) {
      if (bouncyVelX > 0.0f)
        bouncyVelX = input.screenWidth / 2.0f;
      else
        bouncyVelX = -input.screenWidth / 2.0f;
      if (bouncyVelY > 0.0f)
        bouncyVelY = input.screenWidth / 2.0f;
      else
        bouncyVelY = -input.screenWidth / 2.0f;
      bouncySize = input.screenHeight / 10;
    }

    bouncyPosX += input.deltatime * bouncyVelX;
    bouncyPosY += input.deltatime * bouncyVelY;
    if (bouncyPosX < 0.0f) {
      bouncyPosX = -bouncyPosX;
      bouncyVelX = -bouncyVelX;
    } else if (bouncyPosX + bouncySize > input.screenWidth) {
      bouncyPosX -= (bouncyPosX + bouncySize) - input.screenWidth;
      bouncyVelX = -bouncyVelX;
    }
    if (bouncyPosY < 0.0f) {
      bouncyPosY = -bouncyPosY;
      bouncyVelY = -bouncyVelY;
    } else if ((bouncyPosY + bouncySize) > input.screenHeight) {
      bouncyPosY -= (bouncyPosY + bouncySize) - input.screenHeight;
      bouncyVelY = -bouncyVelY;
    }
    PlatformDrawRect(bouncyPosX, bouncyPosY, bouncySize, bouncySize, 0xAAAAAAFF);

    if (input.keydownA && !input.keydownAPrev) {
      colorIndex -= 1;
      if (colorIndex < 0)
        colorIndex += 8;
    }
    if (input.keydownD && !input.keydownDPrev) {
      colorIndex = (colorIndex + 1) % 8;
    }

    if (input.keydownA) {
      PlatformDrawRect(
          0 * input.screenWidth / 10,
          0,
          input.screenWidth / 10,
          input.screenHeight / 10,
          COLOR_LIST[(colorIndex + 1) % 8]
      );
    }
    if (input.keydownS) {
      PlatformDrawRect(
          1 * input.screenWidth / 10,
          0,
          input.screenWidth / 10,
          input.screenHeight / 10,
          COLOR_LIST[(colorIndex + 2) % 8]
      );
    }
    if (input.keydownD) {
      PlatformDrawRect(
          2 * input.screenWidth / 10,
          0,
          input.screenWidth / 10,
          input.screenHeight / 10,
          COLOR_LIST[(colorIndex + 3) % 8]
      );
    }
  }
};

static GameState* gGame = nullptr;
static Input* gInput    = nullptr;

void GameKeyUp(int key) { gInput->OnKeyUp(key); }
void GameKeyDown(int key) { gInput->OnKeyDown(key); }
void GameScreenResize(uint32 width, uint32 height) { gInput->OnScreenResize(width, height); }

void GameInit(uint32 width, uint32 height) {
  gInput = new Input(width, height);
  gGame  = new GameState(*gInput);
  LogInfo("Game initialized");
}

void GameDoFrame(float deltatime) {
  gInput->BeginFrame(deltatime);
  gGame->DoFrame(*gInput);
  gInput->EndFrame();
}