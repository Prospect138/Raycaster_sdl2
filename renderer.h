/*
Renderer provides access to all resources involved in rendering frame 
such as SDL funcs and objects and raycasting algorithms
*/
#pragma once

#include "map.h"
#include "player.h"
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int num_of_rays = SCREEN_WIDTH;


class Renderer {
private:
  //Renderer stores SDL objects:
  SDL_Window* g_window_ = nullptr;
  SDL_Renderer* g_renderer_ = nullptr;

  //And reference to player to have the point of view
  Player* player_;
  
  double CalculateDistance(int i);

public:
  Renderer(Player* player);
  // No consturct without player
  Renderer() = delete;

  bool InitSDL();
  void CloseSDL();

  //Render frame based on map and player's coordinate
  void RenderFrame();

  // Test func only for draw y = x line;
  void RenderLine();
};