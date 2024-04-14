#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "utils.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int map_height = 16;
const int map_width = 16;

const int num_of_rays = 80;
const int pixel_in_stack = 8;

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
	int fov;
};

Player player{5.0, 10.0, 90.0, 80};

bool Init();

bool LoadMedia();

void Close();

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
	g_renderer = nullptrptr;

	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	SDL_Quit();
}

void RenderLine(){
	for (int i = 0; i < SCREEN_WIDTH; i++){
		SDL_RenderDrawPoint(g_renderer, i, i);
	}
}

CompFloat PerformDDA(int i){
	CompFloat angle = player.rotation_angle - (player.fov/2) + ((player.fov / num_of_rays) * i);
	utils::NormalizeAngle(angle);
	angle *= (M_PI / 180);

	//

	CompFloat ray_length = 0.0;
	Point ray_coordinate{player.coordinate.x, player.coordinate.y};

	//int offset shows direction of shifting depending on angle
	int offset_y;

	int offset_x = (( 0 < angle && angle < M_PI/2) || ( 3 * M_PI / 2 < angle && angle < 2 * M_PI )) ? 1 : -1;
	( M_PI/2 < angle && angle < M_PI ) || ( 0 < angle && angle < M_PI/2 ) ? offset_y = 1 : offset_y = -1;

	CompFloat dx;
	CompFloat dist1; 
	switch (offset_x) {
		case(1): {
			dx = ceil(ray_coordinate.x) - ray_coordinate.x;
			dist1 = abs(dx / cos(angle));
		} break;
	
	case -1:
		dx = floor(ray_coordinate.x) - ray_coordinate.x;
		dist1 = abs(dx / cos(angle));
		break;
	}

	CompFloat dy;
	CompFloat dist2;
	switch (offset_y)
	{
	case 1:
		dy = ceil(ray_coordinate.y) - ray_coordinate.y;
		dist2 = abs(dy / sin(angle));
		break;

	case -1:
		dy = floor(ray_coordinate.y) - ray_coordinate.y;
		dist2 = abs(dy / sin(angle));
		break;
	}


	if (dist1 < dist2){
		ray_length += dist1;
		ray_coordinate.x += offset_x * dx;
		ray_coordinate.y += offset_x * dx * tan(angle);
	}
	else{
		ray_length += dist2;
		ray_coordinate.x += offset_y * dy / tan(angle);
		ray_coordinate.y += offset_y * dy; 
	}

	//calculate first cell

	//y = kx + b


	while(ray_length < 150.0){
		dist1 = abs(offset_x / cos(angle));

		dist2 = abs(offset_y / sin(angle));

		if (dist1 < dist2){
			ray_length += dist1;
			ray_coordinate.x += offset_x;
			ray_coordinate.y += offset_x * tan(angle);
			if (map[int(ray_coordinate.x + offset_x)][int(ray_coordinate.y)] == 1){
				break;
			}
		}
		else {
			ray_length += dist2;
			ray_coordinate.x -= offset_y / tan(angle);
			ray_coordinate.y += offset_y;
			if(map[ssize_t(ray_coordinate.x)][ssize_t(ray_coordinate.y + offset_y)] == 1) {
				break;
			};
		}
	}

	return CompFloat(SCREEN_HEIGHT) / (2 * ray_length);

}

void RenderFrame(){
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer);
	SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
	CompFloat lng[num_of_rays];
	int pixel_counter = 0;
	int ray_width = SCREEN_WIDTH / num_of_rays;
	for (int i = 0; i < num_of_rays; i++){
		lng[i] = PerformDDA(i);
		int heigth = lng[i];

		int new_count = pixel_counter + ray_width;

		while (pixel_counter < new_count){

			for(int j = SCREEN_HEIGHT / 2 + 1, jEnd = j + heigth; j < jEnd; j++){
				SDL_RenderDrawPoint(g_renderer, pixel_counter, j);
				SDL_RenderDrawPoint(g_renderer, pixel_counter, SCREEN_HEIGHT + 1 - j);
			}
			pixel_counter += 1;
		}
	}

}

int main( int argc, char* args[] )
{
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
				if (e.type == SDL_QUIT){
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					switch(e.key.keysym.sym)
					{
						case SDLK_LEFT: player.rotation_angle -= 2; break;

						case SDLK_RIGHT:
						player.rotation_angle += 2;
						break;
					}
				}
			}
			utils::NormalizeAngle(player.rotation_angle);
			RenderFrame();
			SDL_RenderPresent(g_renderer);

    	}
	}
	Close();
    return 0;
}
