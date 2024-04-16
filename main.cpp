#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include "utils.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int num_of_rays = SCREEN_WIDTH;

#include "map.h"

typedef double CompFloat;

struct Point
{
	CompFloat x;
	CompFloat y;
};

struct Player {
	Point coordinate;
	CompFloat rotation_angle;
	CompFloat fov;
};

Player player{5.0, 10.0, 90.0, 80};

SDL_Window* g_window = nullptr;

SDL_Renderer* g_renderer = nullptr;
SDL_Renderer* main_renderer = nullptr;

bool Init() {
	bool success = true;
  //init sdl
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("No SDL init\n");
		success = false;
	}
	else {
        //create window
		int check = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &g_window, &g_renderer);
    	SDL_RenderClear(g_renderer);
    	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
		SDL_Delay(1000);
		if (g_window == nullptr || g_renderer == nullptr) {
			printf("no window\n");
			success = false;
		};
	}
	return success;
}

void Close(){
	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;

	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	SDL_Quit();
}

void RenderLine(){
	for (int i = 0; i < SCREEN_WIDTH; i++){
		SDL_RenderDrawPoint(g_renderer, i, i);
	}
}

CompFloat CalculateDist(int i){
	CompFloat angle = player.rotation_angle - (player.fov/2) + ((player.fov / CompFloat(num_of_rays)) * i);
	angle *= ( M_PI / 180 );
	CompFloat ray_length = 0.0;

	for ( ; ray_length < 500.0; ray_length += 0.1) {
		CompFloat dist_x = player.coordinate.x + ray_length * sin(angle);
		CompFloat dist_y = player.coordinate.y + ray_length * cos(angle);

		if ( map[static_cast<int>(dist_y)][static_cast<int>(dist_x)] == 1 ) {
			break;
		}
	}
	CompFloat player_angle = player.rotation_angle * ( M_PI / 180 );
	ray_length *= cos( angle - player_angle );
	return static_cast<CompFloat>(SCREEN_HEIGHT) / ray_length;
}

void RenderFrame() {
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer);
	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
	CompFloat lng[num_of_rays];
	int pixel_counter = 0;
	int ray_width = SCREEN_WIDTH / num_of_rays;
	for (int i = 0; i < num_of_rays; i++){
		lng[i] = CalculateDist(i);
		int heigth = lng[i];
		int new_count = pixel_counter + ray_width;
		
		while (pixel_counter < new_count){
			for(int j = 241; j < 241 + heigth; j++){
				SDL_RenderDrawPoint(g_renderer, pixel_counter, j);
				SDL_RenderDrawPoint(g_renderer, pixel_counter, SCREEN_HEIGHT + 1 - j);
			}
			pixel_counter += 1;
		}
	}
}

int main(int argc, char* args[]) {
	if(!Init()){
		printf("fail on Init()\n");
	}
	else 
	{
		SDL_Event e; 
		bool quit = false;
		
		RenderFrame();
		SDL_RenderPresent(g_renderer);
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
			//utils::NormalizeAngle(player.rotation_angle);
			RenderFrame();
			SDL_RenderPresent(g_renderer);
    }
	}
	Close();
    return 0;
}
