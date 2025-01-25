#ifndef RRENDERTSYSTEM_H
#define RRENDERTSYSTEM_H

#include <SDL.h>
#include <algorithm> 

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetManager/AssetManager.h"

class RenderSystem : public System {
	public:
		RenderSystem() {
			RequireComponent<TransformComponent>();
			RequireComponent<SpriteComponent>();
		}

		void Update(SDL_Renderer* renderer, std::unique_ptr<AssetManager>& assetManager, SDL_Rect& camera) {
			struct RenderableEntity {
				TransformComponent transformComponent;
				SpriteComponent spriteComponent;
			};

			std::vector<RenderableEntity> renderableEntities;

			for (auto& entity : GetSystemEntities()) {
				RenderableEntity renderableEntity;
				renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
				renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
				renderableEntities.emplace_back(renderableEntity);
			}

			std::sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) {
				return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
			});

			for (auto& entity : renderableEntities) {
				auto& transform = entity.transformComponent;
				const auto& sprite = entity.spriteComponent;

				SDL_Rect sourceRectangle = sprite.srcRect;

				SDL_Rect destinationRectangle = {
					static_cast<int>(transform.position.x - camera.x),
					static_cast<int>(transform.position.y - camera.y),
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

			}
		};
};

#endif