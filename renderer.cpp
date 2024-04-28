#include "renderer.h"

Renderer::Renderer(Player* player) { player_ = player; }

bool Renderer::InitSDL() {
	bool success = true;
  //init sdl
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("No SDL init\n");
		success = false;
	}
	else {
    //create window
		int check = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &g_window_, &g_renderer_);
    	SDL_RenderClear(g_renderer_);
    	SDL_SetRenderDrawColor(g_renderer_, 255, 0, 0, 255);
		SDL_Delay(1000);
		if (g_window_ == nullptr || g_renderer_ == nullptr) {
			printf("no window\n");
			success = false;
		};
	}
	return success;
}

void Renderer::CloseSDL() {
	SDL_DestroyRenderer(g_renderer_);
	g_renderer_ = nullptr;

	SDL_DestroyWindow(g_window_);
	g_window_ = nullptr;

	SDL_Quit();
}

void Renderer::RenderFrame() {
	SDL_SetRenderDrawColor(g_renderer_, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer_);
	SDL_SetRenderDrawColor(g_renderer_, 255, 0, 0, 255);
	double lng[num_of_rays];
	int pixel_counter = 0;
	int ray_width = SCREEN_WIDTH / num_of_rays;
	for (int i = 0; i < num_of_rays; i++) {
		lng[i] = Renderer::CalculateDistance(i);
		int heigth = lng[i];
		int new_count = pixel_counter + ray_width;
		
		while (pixel_counter < new_count){
			for(int j = 241; j < 241 + heigth; j++){
				SDL_RenderDrawPoint(g_renderer_, pixel_counter, j);
				SDL_RenderDrawPoint(g_renderer_, pixel_counter, SCREEN_HEIGHT + 1 - j);
			}
			pixel_counter += 1;
		}
	}

  SDL_RenderPresent(g_renderer_);
}

double Renderer::CalculateDistance(int i) {
	double angle = player_ -> rotation_angle - (player_ -> fov/2) + ((player_ -> fov / double(num_of_rays)) * i);
	angle *= ( M_PI / 180 );
	double ray_length = 0.0;

	for ( ; ray_length < 500.0; ray_length += 0.1) {
		double dist_x = player_ -> coordinate.x + ray_length * sin(angle);
		double dist_y = player_ -> coordinate.y + ray_length * cos(angle);

		if ( map[static_cast<int>(dist_y)][static_cast<int>(dist_x)] == 1 ) {
			break;
		}
	}
	double player_angle = player_ -> rotation_angle * ( M_PI / 180 );
	ray_length *= cos( angle - player_angle );
	return static_cast<double>(SCREEN_HEIGHT) / ray_length;
}

void Renderer::RenderLine(){
	for (int i = 0; i < SCREEN_WIDTH; i++){
		SDL_RenderDrawPoint(g_renderer_, i, i);
	}
}