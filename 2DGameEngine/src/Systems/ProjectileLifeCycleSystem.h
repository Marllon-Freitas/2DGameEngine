#ifndef PROJECTILELIFECYCLESYSTEM_H
#define PROJECTILELIFECYCLESYSTEM_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"

class ProjectileLifeCycleSystem : public System {
public:
    ProjectileLifeCycleSystem() {
        RequireComponent<ProjectileComponent>();
    }

    void Update() {
        for (auto& entity : GetSystemEntities()) {
            auto& projectile = entity.GetComponent<ProjectileComponent>();

            if (SDL_GetTicks() - projectile.startTime > projectile.duration) {
                entity.Kill();
            }
        }
    }
};

#endif
