#include "game_engine.h"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const double MOUSE_SENSETIVITY = 0.1;

const int num_of_rays = SCREEN_WIDTH;

//#define LOG("text") std::cerr << "text" << "\n";

void GameEngine::CalculateTrigonimetry(){
	std::cerr << "Calculating trignometric functions\n";
	for (int i = 0; i < 3600; i++) {
		angle_to_cos_[i] = cos(i * 0.1f * M_PI/180);
		angle_to_sin_[i] = sin(i * 0.1f * M_PI/180);
	}
	std::cerr << "Calculating is done\n";
}

double GameEngine::e_sin(double angle)
{
	//utils::NormalizeAngle(angle);
	int _angle = static_cast<int>(round(angle * 10));
	
    return angle_to_sin_[_angle];
}

double GameEngine::e_cos(double angle)
{
	//utils::NormalizeAngle(angle);
    int _angle = static_cast<int>(round(angle * 10));
    return angle_to_cos_[_angle];
}

GameEngine::GameEngine(Player *player)
{
    player_ = player;
	CalculateTrigonimetry();
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

SDL_Texture* GameEngine::LoadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( g_renderer_, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

bool GameEngine::StartMainLoop()
{
    if(!InitSDL()){
		printf("fail on Init()\n");
	}
	else 
	{
		quit_ = false;
		SDL_SetWindowGrab(g_window_, SDL_TRUE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		RenderFrame();

        std::thread thread1( &GameEngine::StartKeyboardHandling, this );
        key_handling_ = std::move( thread1 );
		signed short int mouseDX;

		SDL_Event event;
    	while (quit_ == false) {
			auto start = std::chrono::high_resolution_clock::now();

            //LOG("rendering frame");
            
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
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("No SDL init\n");
		success = false;
	}
	else {
		int check = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &g_window_, &g_renderer_);
    	SDL_RenderClear(g_renderer_);
    	SDL_SetRenderDrawColor(g_renderer_, 255, 0, 0, 255);
		SDL_Delay(1000);
		textures_[0] = LoadTexture("../data/textures/CaveGoldenStones.png");
		textures_[1] = LoadTexture("../data/textures/texture_sample.png");
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
	SDL_Rect texture_rect_src_orig;
	texture_rect_src_orig.x = 0; //the x coordinate
	texture_rect_src_orig.y = 0; //the y coordinate
	texture_rect_src_orig.w = 1024; //the width of the texture
	texture_rect_src_orig.h = 1024; 

	SDL_Rect texture_rect_dst_orig;
	texture_rect_dst_orig.x = 0; //the x coordinate
	texture_rect_dst_orig.y = 0; //the y coordinate
	texture_rect_dst_orig.w = 100; //the width of the texture
	texture_rect_dst_orig.h = 100;

	SDL_SetRenderDrawColor(g_renderer_, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer_);
	SDL_SetRenderDrawColor(g_renderer_, 255, 0, 0, 255);
	double lng[num_of_rays];
	int pixel_counter = 0;
	int ray_width = SCREEN_WIDTH / num_of_rays;

	double prev_dist_x = 0.0;
	double prev_dist_y = 0.0;

	for (int i = 0; i < num_of_rays; i++) {
		double hit;
		double angle = player_ -> GetRotationAngle() - (player_ -> fov/2) + ((player_ -> fov / double(num_of_rays)) * i);
		utils::NormalizeAngle(angle);

		double ray_length = 0.0;
		double dist_x, dist_y;

		for ( ; ray_length < 500.0; ray_length++) {

			dist_x = player_ -> coordinate.x + ray_length * e_sin(angle);
			dist_y = player_ -> coordinate.y + ray_length * e_cos(angle);

			if ( map[static_cast<int>(dist_y)][static_cast<int>(dist_x)] != 0 ) {
				ray_length-=1;
				break;	
			}
		}
		for ( ; ray_length < 500.0; ray_length+=0.01) {
			dist_x = player_ -> coordinate.x + ray_length * e_sin(angle);
			dist_y = player_ -> coordinate.y + ray_length * e_cos(angle);
			if ( map[static_cast<int>(dist_y)][static_cast<int>(dist_x)] != 0 ) {
				break;	
			}
		}
		if (abs(prev_dist_x - dist_x) > abs(prev_dist_y - dist_y)) {
			if (dist_x < prev_dist_x) {
				hit = abs(dist_x - ceil(dist_x));
			}
			else {
				hit = abs(dist_x - floor(dist_x));
			}
		}
		else if (abs(dist_x - prev_dist_x) < abs(dist_y - prev_dist_y)) {
			if (dist_y > prev_dist_y) {
				hit = abs(dist_y - floor(dist_y));
			}
			else {
				hit = abs(dist_y - ceil(dist_y));
			}
		}
		prev_dist_x = dist_x;
		prev_dist_y = dist_y;
		double player_angle = player_ -> GetRotationAngle(); 
		lng[i] = ray_length;
		double odd_angle = angle - player_angle;
		utils::NormalizeAngle(odd_angle);
		lng[i]  *= e_cos(odd_angle);
		lng[i]  = static_cast<double>(SCREEN_HEIGHT) / lng[i] ;

		SDL_Rect texture_rect_src;
		texture_rect_src.x = 1024 * hit;
		texture_rect_src.y = 0; //the y coordinate
		texture_rect_src.w = 100; //the width of the texture
		texture_rect_src.h = 1024;

		SDL_Rect texture_rect_dst;
		texture_rect_dst.x = i; //the x coordinate
		texture_rect_dst.y = SCREEN_HEIGHT/2 - lng[i]/2; //the y coordinate
		texture_rect_dst.w = 1; //the width of the texture
		texture_rect_dst.h = lng[i];
		//std::cerr << map[static_cast<int>(dist_y)][static_cast<int>(dist_x)] << std::endl;
		switch (map[static_cast<int>(dist_y)][static_cast<int>(dist_x)]) {
		case 1:
			SDL_RenderCopy(g_renderer_, textures_[0], &texture_rect_src, &texture_rect_dst);
			break;
		case 2:
			SDL_RenderCopy(g_renderer_, textures_[1], &texture_rect_src, &texture_rect_dst);
			break;
		default:
			break;
		}

		//SDL_RenderCopy(g_renderer_, textures_[0], &texture_rect_src, &texture_rect_dst); 

	}
	SDL_RenderCopy(g_renderer_, textures_[1], &texture_rect_src_orig, &texture_rect_dst_orig);
  	SDL_RenderPresent(g_renderer_);
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
        //std::cout << "Process Move Forward\n";
        Point new_coordinate;
		new_coordinate.x = player_ -> coordinate.x + player_ -> speed * e_sin( player_ -> GetRotationAngle()  );
		new_coordinate.y = player_ -> coordinate.y + player_ -> speed * e_cos( player_ -> GetRotationAngle() );
		if (map[static_cast<int>(new_coordinate.y)][static_cast<int>(new_coordinate.x)] == 0) {
			player_ -> coordinate.x = new_coordinate.x;
			player_ -> coordinate.y = new_coordinate.y;
		}
    }

    if (handler_.key_states[SDL_SCANCODE_S])
    {
        //std::cout << "Process Move back\n";
        Point new_coordinate;
		new_coordinate.x = player_ -> coordinate.x - player_ -> speed * e_sin( player_ -> GetRotationAngle() );
		new_coordinate.y = player_ -> coordinate.y - player_ -> speed * e_cos( player_ -> GetRotationAngle() );
		if (map[static_cast<int>(new_coordinate.y)][static_cast<int>(new_coordinate.x)] == 0) {
			player_ -> coordinate.x = new_coordinate.x;
			player_ -> coordinate.y = new_coordinate.y;
		}
    }

    if (handler_.key_states[SDL_SCANCODE_A])
    {
        Point new_coordinate;
		double new_angle = player_ -> GetRotationAngle() - 90;
		utils::NormalizeAngle(new_angle);
		new_coordinate.x = player_ -> coordinate.x + player_ -> speed * e_sin( new_angle );
		new_coordinate.y = player_ -> coordinate.y + player_ -> speed * e_cos( new_angle );
		if (map[static_cast<int>(new_coordinate.y)][static_cast<int>(new_coordinate.x)] == 0) {
			player_ -> coordinate.x = new_coordinate.x;
			player_ -> coordinate.y = new_coordinate.y;
		}
    }

    if (handler_.key_states[SDL_SCANCODE_D])
    {
        Point new_coordinate;
		double new_angle = player_ -> GetRotationAngle() + 90;
		utils::NormalizeAngle(new_angle);
		new_coordinate.x = player_ -> coordinate.x + player_ -> speed * e_sin( new_angle );
		new_coordinate.y = player_ -> coordinate.y + player_ -> speed * e_cos( new_angle );
		if (map[static_cast<int>(new_coordinate.y)][static_cast<int>(new_coordinate.x)] == 0) {
			player_ -> coordinate.x = new_coordinate.x;
			player_ -> coordinate.y = new_coordinate.y;
		}
    }

	if (handler_.key_states[SDL_SCANCODE_ESCAPE])
	{
		quit_ = true;
		key_handling_.detach();
	}

	if (abs(handler_.mouse_dx) > 0) {
		player_->Rotate (handler_.mouse_dx * MOUSE_SENSETIVITY);
		handler_.mouse_dx = 0;
	}
}

KeyboardHandler::KeyboardHandler()
{
};

int KeyboardHandler::HandleInput(SDL_Event event)
{   
    switch (event.type)
	{
        case SDL_KEYDOWN:
            //std::cerr << "Handle keydown\n";
            key_states[event.key.keysym.scancode] = true;
            break;
        case SDL_KEYUP:
            //std::cerr << "Handle keyup\n";
            key_states[event.key.keysym.scancode] = false;
            break;
		case SDL_MOUSEMOTION:
			//std::cerr << mouse_dx << std::endl;
			mouse_dx = event.motion.xrel;
			break;
	}
    return 0;
}
