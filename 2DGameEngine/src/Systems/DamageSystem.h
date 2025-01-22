#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/CollisionEvent.h"
#include "../Logger/Logger.h"

class DamageSystem : public System {
	public:
		DamageSystem() {
			RequireComponent<BoxColliderComponent>();
		}

		void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
			eventBus->SubscribeToEvent<CollisionEvent>(this, &DamageSystem::onCollision);
		}

		void onCollision(CollisionEvent& event) {
			Logger::Log("The Damage System received an event collision between enities " + std::to_string(event.entityA.GetId()) + " and " + std::to_string(event.entityB.GetId()));
			event.entityA.Kill();
			event.entityB.Kill();
		}

		void Update(std::unique_ptr<EventBus>& eventBus) {
			auto entities = GetSystemEntities();
		}
};

#endif