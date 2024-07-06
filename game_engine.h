#include "player.h"
#include "map.h"

#include <map>
#include <functional>
#include <SDL2/SDL.h>
#include <thread>
#include <chrono>
#include <iostream>

class KeyboardHandler {

friend class GameEngine;

public:
    KeyboardHandler();

    int HandleInput(SDL_Event event);
        
private:
    std::map<SDL_Scancode, bool> key_states;
};

class GameEngine {
public:
    GameEngine() = default;

    GameEngine(Player *player);

    bool StartMainLoop();

    bool InitSDL();
    void CloseSDL();

    void ProcessInput();
    //Render frame based on map and player's coordinate
    void RenderFrame();   
    // Test func only for draw y = x line;
    void RenderLine();
    void StartKeyboardHandling();

private:

    KeyboardHandler handler_;
      //Renderer stores SDL objects:
    SDL_Window* g_window_ = nullptr;
    SDL_Renderer* g_renderer_ = nullptr;    
    //And player's reference that defines the point of view
    Player* player_;
    std::thread key_handling_;

    double CalculateDistance(int i) const;

    bool quit_;

};