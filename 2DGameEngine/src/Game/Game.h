#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "../ECS/ECS.h"

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
	private:
		bool m_isRuning;
		int m_millisecondsPreviuosFrame = 0;
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;

		std::unique_ptr<Registry> m_registry;

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