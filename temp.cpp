#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

bool Init();

bool LoadMedia();

void Close();

SDL_Surface* GLoadSurface(std::string path);

SDL_Window* g_window = NULL;

SDL_Surface* g_screen_surface = NULL;

SDL_Surface* g_current_surface = NULL;

SDL_Surface* g_press_surface[KeyPressSurfaces::KEY_PRESS_SURFACE_TOTAL];

SDL_Texture* load_texture(std::string path);

SDL_Texture* g_texture = NULL;

SDL_Renderer* g_renderer = NULL;

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
		//g_window = SDL_CreateWindow("Cute Kitten", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		SDL_Delay(1000);
		if (g_window == NULL) {
			printf("no window\n");
			success = false;
		}
		else{
            int img_flags = IMG_INIT_PNG;
            if (!(IMG_Init(img_flags) & img_flags)){
                printf("can't init SDL_image\n");
                success = false;
            }
            else{
                g_screen_surface = SDL_GetWindowSurface(g_window);

            }
			
		}
	}
	return success;
}

SDL_Surface* GLoadSurface(std::string path) {
	SDL_Surface* optimized_surface = NULL;
	SDL_Surface* new_surface = IMG_Load(path.c_str());
	if (g_screen_surface == NULL) {
		printf("No image\n");
	}
	else {
		optimized_surface = SDL_ConvertSurface(new_surface, g_screen_surface->format, 0);
		if (optimized_surface == NULL){
			printf("Fucked up surface conversion\n");
		}
		SDL_FreeSurface(new_surface);
	}
	return optimized_surface;
}

bool LoadMedia(){
	bool success = true;
	g_press_surface[KEY_PRESS_SURFACE_TOTAL] = GLoadSurface("04_key_presses/press.bmp");
	if (g_press_surface[KEY_PRESS_SURFACE_TOTAL] == NULL) {
		printf("No image\n");
		success = false;
	}

	g_press_surface[KEY_PRESS_SURFACE_RIGHT] = GLoadSurface("04_key_presses/right.bmp");
	if (g_press_surface[KEY_PRESS_SURFACE_RIGHT] == NULL) {
		printf("No image\n");
		success = false;
	}

	g_press_surface[KEY_PRESS_SURFACE_LEFT] = GLoadSurface("04_key_presses/left.bmp");
	if (g_press_surface[KEY_PRESS_SURFACE_LEFT] == NULL) {
		printf("No image\n");
		success = false;
	}

	g_press_surface[KEY_PRESS_SURFACE_UP] = GLoadSurface("04_key_presses/up.bmp");
	if (g_press_surface[KEY_PRESS_SURFACE_UP] == NULL) {
		printf("No image\n");
		success = false;
	}
		g_press_surface[KEY_PRESS_SURFACE_DOWN] = GLoadSurface("04_key_presses/down.bmp");
	if (g_press_surface[KEY_PRESS_SURFACE_DOWN] == NULL) {
		printf("No image\n");
		success = false;
	}
	return success;
}

void Close(){
	SDL_FreeSurface(g_current_surface);
	g_current_surface = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	SDL_Quit();
}
int main( int argc, char* args[] )
{


	if(!Init()){
		printf("fail on Init()\n");
	}
	else 
	{
		if (!LoadMedia()){
			printf("fail to load media\n");
		}
		else{
			SDL_Event e; 
			bool quit = false;
			g_current_surface = g_press_surface[KEY_PRESS_SURFACE_DEFAULT];	
			while( quit == false )
			{ 
				while( SDL_PollEvent( &e ) )
				{ 
					if( e.type == SDL_QUIT ) 
					{
						quit = true; 
					}
					else if (e.type == SDL_KEYDOWN)
					{
						switch(e.key.keysym.sym)
						{
							case SDLK_UP:
							g_current_surface = g_press_surface[KEY_PRESS_SURFACE_UP];
							break;

							case SDLK_DOWN:
        	                g_current_surface = g_press_surface[ KEY_PRESS_SURFACE_DOWN ];
        	                break;

        	                case SDLK_LEFT:
        	                g_current_surface = g_press_surface[ KEY_PRESS_SURFACE_LEFT ];
        	                break;

        	                case SDLK_RIGHT:
        	                g_current_surface = g_press_surface[ KEY_PRESS_SURFACE_RIGHT ];
        	                break;

        	                default:
        	                g_current_surface = g_press_surface[ KEY_PRESS_SURFACE_DEFAULT ];
        	                break;
						}

					}
				}
				SDL_Rect stretchRect;
                stretchRect.x = 0;
                stretchRect.y = 0;
                stretchRect.w = SCREEN_WIDTH;
                stretchRect.h = SCREEN_HEIGHT;
                
				SDL_BlitSurface(g_current_surface, NULL, g_screen_surface, NULL);
				SDL_UpdateWindowSurface(g_window);
			}

		}
		
	}
	Close();
    return 0;
}