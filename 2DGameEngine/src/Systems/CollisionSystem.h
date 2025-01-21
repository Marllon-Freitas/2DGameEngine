#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"

class CollisionSystem : public System {
public:
	CollisionSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	};

	void Update() {
		auto entities = GetSystemEntities();

		for (auto i = entities.begin(); i != entities.end(); i++) {
			Entity a = *i;
			auto& aTranform = a.GetComponent<TransformComponent>();
			auto& aCollider = a.GetComponent<BoxColliderComponent>();

			for (auto j = i; j != entities.end(); j++) {
				Entity b = *j;

				if (a == b) continue;

				auto& bTranform = b.GetComponent<TransformComponent>();
				auto& bCollider = b.GetComponent<BoxColliderComponent>();

				bool isCollisionHappened = this->CheckAABBCollision(
					aTranform.position.x + aCollider.offSet.x,
					aTranform.position.y + aCollider.offSet.y,
					aCollider.width,
					aCollider.height,
					bTranform.position.x + bCollider.offSet.x,
					bTranform.position.y + bCollider.offSet.y,
					bCollider.width,
					bCollider.height
				);

				if (isCollisionHappened) {
					Logger::Success("Entity " + std::to_string(a.GetId()) + " is coliding with entity " + std::to_string(b.GetId()));
					a.Kill();
					b.Kill();
				}
			}
		}
	};

	bool CheckAABBCollision(
		double aX, double aY, double aW, double aH,
		double bX, double bY, double bW, double bH
	) {
		return (
			aX < bX + bW &&
			aX + aW > bX &&
			aY < bY + bH &&
			aY + aH > bY
		);
	}
};

#endif