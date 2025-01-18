#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>

#include "SDL.h"

class AssetManager {
	private:
		std::map<std::string, SDL_Texture*> m_textures;

	public:
		AssetManager();
		~AssetManager();

		void ClearAssets();
		void AddTexture(const std::string& assetId, const std::string& filePath, SDL_Renderer* renderer);
		SDL_Texture* GetTexture(const std::string& assetId);
};

#endif