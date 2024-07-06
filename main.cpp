#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include "map.h"
#include "utils.h"
#include "game_engine.h"

int main(int argc, char* args[]) {

  Player player{{5.0, 10.0}, 90.0, 80};

  GameEngine engine(&player);

  engine.StartMainLoop();
  
  return 0;
}
