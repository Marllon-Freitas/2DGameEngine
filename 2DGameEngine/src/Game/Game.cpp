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
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/CameraFollowComponent.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyBoardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game() {
    m_isRuning = false;
    m_isDebug = false;
    m_registry = std::make_unique<Registry>();
    m_assetManager = std::make_unique<AssetManager>();
    m_eventBus = std::make_unique<EventBus>();
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

    m_camera = { 0, 0, windowWidth, windowHeight };

    SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

    m_isRuning = true;
}

void Game::LoadLevel(int level) {
    m_registry->AddSystem<MovementSystem>();
    m_registry->AddSystem<RenderSystem>();
    m_registry->AddSystem<AnimationSystem>();
    m_registry->AddSystem<CollisionSystem>();
    m_registry->AddSystem<RenderColliderSystem>();
    m_registry->AddSystem<DamageSystem>();
    m_registry->AddSystem<KeyboardControlSystem>();
    m_registry->AddSystem<CameraMovementSystem>();

    m_assetManager->AddTexture("tank-image", "./assets/images/tank-panther-right.png", m_renderer);
    m_assetManager->AddTexture("truck-image", "./assets/images/truck-ford-right.png", m_renderer);
    m_assetManager->AddTexture("chopper-image", "./assets/images/chopper-spritesheet.png", m_renderer);
    m_assetManager->AddTexture("tilemap-image", "./assets/tilemaps/jungle.png", m_renderer);

    int tileSize = 32;
    double tileScale = 3.0;
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
            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
        }
    }
    mapFile.close();

    mapWidth = mapNumCols * tileSize * tileScale;
    mapHeight = mapNumRows * tileSize * tileScale;

    Entity tank = m_registry->CreateEntity();

    tank.AddComponent<TransformComponent>(glm::vec2(340.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1, true);
    tank.AddComponent<BoxColliderComponent>(32, 32);

    Entity truck = m_registry->CreateEntity();

    truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 2);
    truck.AddComponent<BoxColliderComponent>(32, 32);

    Entity chopper = m_registry->CreateEntity();

    chopper.AddComponent<TransformComponent>(glm::vec2(10.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 3);
    chopper.AddComponent<AnimationComponent>(2, 12, true);
    chopper.AddComponent<BoxColliderComponent>(32, 32);
    chopper.AddComponent<CameraFollowComponent>();
    chopper.AddComponent<KeyboardControlComponent>(
        glm::vec2(0, -100),
        glm::vec2(100, 0),
        glm::vec2(0, 100),
        glm::vec2(-100, 0)
    );

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
                if (sdlEvent.key.keysym.sym == SDLK_d) {
                    m_isDebug = !m_isDebug;
                }
                m_eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
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

    m_eventBus->Reset();

    m_registry->GetSystem<DamageSystem>().SubscribeToEvents(m_eventBus);
    m_registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(m_eventBus);

    // update the registry to process entities that are waiting to be created/deleted
    m_registry->Update();

    // update all the systems
    m_registry->GetSystem<MovementSystem>().Update(deltaTime);
    m_registry->GetSystem<AnimationSystem>().Update();
    m_registry->GetSystem<CollisionSystem>().Update(m_eventBus);
    m_registry->GetSystem<CameraMovementSystem>().Update(m_camera);
}

void Game::Render() {
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);

    m_registry->GetSystem<RenderSystem>().Update(m_renderer, m_assetManager, m_camera);
    if (m_isDebug) {
        m_registry->GetSystem<RenderColliderSystem>().Update(m_renderer);
    }

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
