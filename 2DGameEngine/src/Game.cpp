#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"

Game::Game() {
    isRuning = false;
    std::cout << "Game Contructor Called!" << std::endl;
}
Game::~Game() {
    std::cout << "Game Destructor Called!" << std::endl;
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    windowWidth = displayMode.w;
    windowHeight = displayMode.h;

    window = SDL_CreateWindow(
        "2DGameEngine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    isRuning = true;
}

void Game::ProcessInput() {
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                isRuning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
                    isRuning = false;
                }
                break;
        }
    }
}

void Game::Setup() {}

void Game::Update() {}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect destinationRect = {
        10, 10,
        32, 32
    };
    SDL_RenderCopy(renderer, texture, NULL, &destinationRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void Game::Run() {
    while (isRuning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
