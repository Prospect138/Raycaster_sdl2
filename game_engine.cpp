#include "game_engine.h"
#include "action.cpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

const int num_of_rays = SCREEN_WIDTH;

GameEngine::GameEngine(Player *player)
{
    player_ = player;
}

void GameEngine::StartKeyboardHandling()
{
    SDL_Event e; 
    while (true) 
    {
        if (SDL_PollEvent(&e))         
        {
            handler_.HandleInput(e);
        }
    }

}

bool GameEngine::StartMainLoop()
{
    if(!InitSDL()){
		printf("fail on Init()\n");
	}
	else 
	{
		quit_ = false;
		
		RenderFrame();
        std::thread thread1( &GameEngine::StartKeyboardHandling, this );
        key_handling_ = std::move( thread1 );

        //int64_t start = SDL_GetTick64();now();

    	while (quit_ == false) {

            std::cout << "rendering frame...\n";
            
            auto start = std::chrono::high_resolution_clock::now();

            ProcessInput();
		    
            RenderFrame();

            if (SCREEN_TICKS_PER_FRAME - start.time_since_epoch().count() > 0) {
                SDL_Delay(SCREEN_TICKS_PER_FRAME - start.time_since_epoch().count() );
            }
        }
	}
}

bool GameEngine::InitSDL() {
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

void GameEngine::CloseSDL() {
	SDL_DestroyRenderer(g_renderer_);
	g_renderer_ = nullptr;

	SDL_DestroyWindow(g_window_);
	g_window_ = nullptr;

	SDL_Quit();
}

void GameEngine::RenderFrame() {
	SDL_SetRenderDrawColor(g_renderer_, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer_);
	SDL_SetRenderDrawColor(g_renderer_, 255, 0, 0, 255);
	double lng[num_of_rays];
	int pixel_counter = 0;
	int ray_width = SCREEN_WIDTH / num_of_rays;
	for (int i = 0; i < num_of_rays; i++) {
		lng[i] = CalculateDistance(i);
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

double GameEngine::CalculateDistance(int i) const {
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

void GameEngine::RenderLine(){
	for (int i = 0; i < SCREEN_WIDTH; i++){
		SDL_RenderDrawPoint(g_renderer_, i, i);
	}
}

void GameEngine::ProcessInput() 
{
    if (handler_.key_states[SDL_SCANCODE_W])
    {
        std::cout << "Process Move Forward\n";
        Point new_coordinate;
		new_coordinate.x = player_ -> coordinate.x + player_ -> speed * sin( player_ -> rotation_angle * (M_PI / 180) );
		new_coordinate.y = player_ -> coordinate.y + player_ -> speed * cos( player_ -> rotation_angle * (M_PI / 180) );
		if (map[static_cast<int>(new_coordinate.y)][static_cast<int>(new_coordinate.x)] == 0) {
			player_ -> coordinate.x = new_coordinate.x;
			player_ -> coordinate.y = new_coordinate.y;
		}
    }

    if (handler_.key_states[SDL_SCANCODE_S])
    {
        std::cout << "Process Move back\n";
        Point new_coordinate;
		new_coordinate.x = player_ -> coordinate.x - player_ -> speed * sin( player_ -> rotation_angle * (M_PI / 180) );
		new_coordinate.y = player_ -> coordinate.y - player_ -> speed * cos( player_ -> rotation_angle * (M_PI / 180) );
		if (map[static_cast<int>(new_coordinate.y)][static_cast<int>(new_coordinate.x)] == 0) {
			player_ -> coordinate.x = new_coordinate.x;
			player_ -> coordinate.y = new_coordinate.y;
		}
    }

    if (handler_.key_states[SDL_SCANCODE_A])
    {
        std::cout << "Process look left\n";
        player_ -> rotation_angle -= 2;
    }

    if (handler_.key_states[SDL_SCANCODE_D])
    {
        std::cout << "Process Look right\n";
        player_ -> rotation_angle += 2;
    }

	if (handler_.key_states[SDL_SCANCODE_ESCAPE])
	{
		quit_ = true;
		key_handling_.detach();
	}
}

KeyboardHandler::KeyboardHandler()
{
};

int KeyboardHandler::HandleInput(SDL_Event event)
{   
    switch (event.type){
        case SDL_KEYDOWN:
            std::cout << "Handle keydown\n";
            key_states[event.key.keysym.scancode] = true;
            break;
        case SDL_KEYUP:
            std::cout << "Handle keyup\n";
            key_states[event.key.keysym.scancode] = false;
            break;
    	}
    return 0;
}
