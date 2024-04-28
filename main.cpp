#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include "map.h"
#include "utils.h"
#include "renderer.h"

int main(int argc, char* args[]) {

  Player player{{5.0, 10.0}, 90.0, 80};

  Renderer renderer(&player);

	if(!renderer.InitSDL()){
		printf("fail on Init()\n");
	}
	else 
	{
		SDL_Event e; 
		bool quit = false;
		
		renderer.RenderFrame();

    	while (quit == false) {
    		while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					switch(e.key.keysym.sym)
					{
						case SDLK_LEFT: player.rotation_angle -= 2; break;

						case SDLK_RIGHT: player.rotation_angle += 2; break;

						case SDLK_UP: {
							Point new_coordinate;
							new_coordinate.x = player.coordinate.x + 0.2 * sin( player.rotation_angle * (M_PI / 180) );
							new_coordinate.y = player.coordinate.y + 0.2 * cos( player.rotation_angle * (M_PI / 180) );

							if (map[static_cast<int>(new_coordinate.y)][static_cast<int>(new_coordinate.x)] == 0) {
								player.coordinate.x = new_coordinate.x;
								player.coordinate.y = new_coordinate.y;
							}
							break;
						}

						case SDLK_DOWN: {
							Point new_coordinate;
							new_coordinate.x = player.coordinate.x - 0.2 * sin( player.rotation_angle * (M_PI / 180) );
							new_coordinate.y = player.coordinate.y - 0.2 * cos( player.rotation_angle * (M_PI / 180) );
							if (map[static_cast<int>(new_coordinate.y)][static_cast<int>(new_coordinate.x)] == 0) {
								player.coordinate.x = new_coordinate.x;
								player.coordinate.y = new_coordinate.y;
							}
							break;					
						}
					}
				}
			}
		  renderer.RenderFrame();
    }
	}
	renderer.CloseSDL();
  return 0;
}
