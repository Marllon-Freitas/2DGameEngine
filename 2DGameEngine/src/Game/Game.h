#ifndef GAME_H
#define GAME_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../EventBus/EventBus.h"

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class Game {
	private:
		bool m_isRuning;
		bool m_isDebug;
		int m_millisecondsPreviuosFrame = 0;
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Rect m_camera;

		std::unique_ptr<Registry> m_registry;
		std::unique_ptr<AssetManager> m_assetManager;
		std::unique_ptr<EventBus> m_eventBus;

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
		void LoadLevel(int level);

		static int windowWidth;
		static int windowHeight;
		static int mapWidth;
		static int mapHeight;
};
#endif