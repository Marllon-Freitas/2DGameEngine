#include <SDL_image.h>

#include "../AssetManager/AssetManager.h"
#include "../Logger/Logger.h"

AssetManager::AssetManager() {
	Logger::Success("Asset Manager constructor called!");
}

AssetManager::~AssetManager() {
	ClearAssets();
	Logger::Success("Asset Manager destructor called!");
}

void AssetManager::ClearAssets() {
	for (auto& texture : m_textures) {
		SDL_DestroyTexture(texture.second);
	}

	m_textures.clear();
}

void AssetManager::AddTexture(const std::string& assetId, const std::string& filePath, SDL_Renderer* renderer) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	m_textures.emplace(assetId, texture);
	Logger::Log("New asset added to the Asset Manager with id = " + assetId);
}

SDL_Texture* AssetManager::GetTexture(const std::string& assetId) {
	return m_textures[assetId];
}