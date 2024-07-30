#include "player.h"
#include "map.h"
#include "utils.h"

#include <map>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
    int mouse_dx;
};

class GameEngine {
public:
    GameEngine() = default;

    GameEngine(Player *player);

    SDL_Texture* LoadTexture( std::string path );

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
    
    SDL_Texture* textures_[4];

    void CalculateTrigonimetry();
    double e_sin(double angle);
    double e_cos(double angle);
    double angle_to_cos_[3600] ;
    double angle_to_sin_[3600] ;

    bool quit_;

};