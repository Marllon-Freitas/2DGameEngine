#ifndef GAME_H
#define GAME_H

#include <SDL.h>

class Game {
	private:
		bool isRuning;
		SDL_Window* window;
		SDL_Renderer* renderer;

	public:
		Game();
		~Game();

		void Initialize();
		void Run();
		void Setup();
		void ProcessInput();
		void Update();
		void Render();
		void Destroy();

		int windowWidth;
		int windowHeight;
};
#endif