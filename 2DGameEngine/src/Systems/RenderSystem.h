#ifndef RRENDERTSYSTEM_H
#define RRENDERTSYSTEM_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetManager/AssetManager.h"

class RenderSystem : public System {
	public:
		RenderSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<SpriteComponent>();
		};

		void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager) {
			for (auto& entity : GetSystemEntities()) {
				auto& transform = entity.GetComponent<TransformComponent>();
				const auto& sprite = entity.GetComponent<SpriteComponent>();

				SDL_Rect sourceRectangle = sprite.srcRect;

				SDL_Rect destinationRectangle = {
					static_cast<int>(transform.position.x),
					static_cast<int>(transform.position.y),
					static_cast<int>(sprite.width * transform.scale.x),
					static_cast<int>(sprite.height * transform.scale.y)
				};

				SDL_RenderCopyEx(
					renderer,
					assetManager->GetTexture(sprite.assetId),
					&sourceRectangle,
					&destinationRectangle,
					transform.rotation,
					NULL,
					SDL_FLIP_NONE
				);

			};
		};
};

#endif