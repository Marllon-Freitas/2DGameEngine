#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>

#include "../ECS/ECS.h"
#include "Game.h"
#include "../Logger/Logger.h"

Game::Game() {
    m_isRuning = false;
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
    // TODO:
    // Entity tank = registry.CreateEntity();
    // tank.AddComponent<TransformComponnet>();
    // tank.AddComponent<BoxColliderComponent>();
    // tank.AddComponent<SpriteComponent>("./assets/images/tank-tiger-right.png");
}

void Game::Update() {
    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - m_millisecondsPreviuosFrame);

    if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    double deltaTime = (SDL_GetTicks() - m_millisecondsPreviuosFrame) / 1000.0;

    m_millisecondsPreviuosFrame = SDL_GetTicks();

    // TODO:
    // MovementSystem.Update();
    // CollisionSystem.Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);

    // TODO: Render Game objects

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
