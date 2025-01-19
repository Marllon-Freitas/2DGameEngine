#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <fstream>

#include "Game.h"
#include "../ECS/ECS.h"
#include "../AssetManager/AssetManager.h"
#include "../Logger/Logger.h"

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"

Game::Game() {
    m_isRuning = false;
    m_registry = std::make_unique<Registry>();
    m_assetManager = std::make_unique<AssetManager>();
    Logger::Success("Game Constructor Called!");
}

Game::~Game() {
    Logger::Success("Game Destructor Called!");
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::Error("Error initializing SDL");
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    windowWidth = displayMode.w;
    windowHeight = displayMode.h;

    m_window = SDL_CreateWindow(
        "2DGameEngine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );

    if (!m_window) {
        Logger::Error("Error creating SDL window.");
        return;
    }

    m_renderer = SDL_CreateRenderer(
        m_window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!m_renderer) {
        Logger::Error("Error creating SDL renderer.");
        return;
    }

    SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

    m_isRuning = true;
}

void Game::LoadLevel(int level) {
    m_registry->AddSystem<MovementSystem>();
    m_registry->AddSystem<RenderSystem>();

    m_assetManager->AddTexture("tank-image", "./assets/images/tank-panther-right.png", m_renderer);
    m_assetManager->AddTexture("truck-image", "./assets/images/truck-ford-right.png", m_renderer);
    m_assetManager->AddTexture("tilemap-image", "./assets/tilemaps/jungle.png", m_renderer);

    int tileSize = 32;
    double tileScale = 2.0;
    int mapNumCols = 25;
    int mapNumRows = 20;
    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");

    if (!mapFile.is_open()) {
        Logger::Error("Failed to open tilemap file: ./assets/tilemaps/jungle.map");
        return;
    }

    for (int y = 0; y < mapNumRows; y++) {
        for (int x = 0; x < mapNumCols; x++) {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = m_registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);
        }
    }
    mapFile.close();

    Entity tank = m_registry->CreateEntity();

    tank.AddComponent<TransformComponent>(glm::vec2(4.0, 0.0), glm::vec2(3.0, 3.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(10.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);

    Entity truck = m_registry->CreateEntity();

    truck.AddComponent<TransformComponent>(glm::vec2(4.0, 0.0), glm::vec2(3.0, 3.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(10.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 2);
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                m_isRuning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    m_isRuning = false;
                }
                break;
        }
    }
}

void Game::Setup() {
    LoadLevel(1);
}

void Game::Update() {
    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - m_millisecondsPreviuosFrame);

    if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - m_millisecondsPreviuosFrame) / 1000.0;

    m_millisecondsPreviuosFrame = SDL_GetTicks();

    // update the registry to process entities that are waiting to be created/deleted
    m_registry->Update();

    // update all the systems
    m_registry->GetSystem<MovementSystem>().Update(deltaTime);
}

void Game::Render() {
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);

    m_registry->GetSystem<RenderSystem>().Update(m_renderer, m_assetManager);

    SDL_RenderPresent(m_renderer);
}

void Game::Run() {
    Setup();
    while (m_isRuning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
