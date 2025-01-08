#include "Game.h"
#include <iostream>

Game::Game() {
    std::cout << "Game Contructor Called!" << std::endl;
}
Game::~Game() {
    std::cout << "Game Destructor Called!" << std::endl;
}

void Game::Initialize() {}

void Game::Run() {}

void Game::ProcessInput() {}

void Game::Update() {}

void Game::Render() {}

void Game::Destroy() {}
