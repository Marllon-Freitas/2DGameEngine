#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>

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
    m_registry->AddSystem<MovementSystem>();
    m_registry->AddSystem<RenderSystem>();

    m_assetManager->AddTexture("tank-image", "./assets/images/tank-panther-right.png", m_renderer);
    m_assetManager->AddTexture("truck-image", "./assets/images/truck-ford-right.png", m_renderer);

    Entity tank = m_registry->CreateEntity();

    tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(40.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32);

    Entity truck = m_registry->CreateEntity();
    
    truck.AddComponent<TransformComponent>(glm::vec2(5.0, 30.0), glm::vec2(3.0, 3.0), 45.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 50.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32);
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
