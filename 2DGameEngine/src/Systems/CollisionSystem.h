#ifndef BOXCOLLIDERSYSTEM_H
#define BOXCOLLIDERSYSTEM_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class AnimationSystem : public System {
public:
	AnimationSystem() {
		RequireComponent<BoxColliderComponent>();
	};

	void Update() {
		for (auto& entity : GetSystemEntities()) {
			//TODO
		}
	};
};

#endif