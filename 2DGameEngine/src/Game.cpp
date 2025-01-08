#include <iostream>
#include <SDL.h>
#include "Game.h"

Game::Game() {
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

    SDL_Window* window = SDL_CreateWindow(
        "2DGameEngine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_BORDERLESS
    );

    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }
}

void Game::Run() {}

void Game::ProcessInput() {}

void Game::Update() {}

void Game::Render() {}

void Game::Destroy() {}
