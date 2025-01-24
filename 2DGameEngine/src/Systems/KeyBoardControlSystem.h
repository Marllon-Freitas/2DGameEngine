#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardControlSystem : public System {
	public:
		KeyboardControlSystem() {
			RequireComponent<SpriteComponent>();
			RequireComponent<RigidBodyComponent>();
			RequireComponent<KeyboardControlComponent>();
		}

		void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
			eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
		}

		void OnKeyPressed(KeyPressedEvent& event) {
			for (auto& entity : GetSystemEntities()) {
				auto& keyboardControl = entity.GetComponent<KeyboardControlComponent>();
				auto& sprite = entity.GetComponent<SpriteComponent>();
				auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

				switch (event.symbol) {
					case SDLK_UP:
						rigidBody.velocity = keyboardControl.upVelocity;
						sprite.srcRect.y = sprite.height * 0;
						break;
					case SDLK_RIGHT:
						rigidBody.velocity = keyboardControl.rightVelocity;
						sprite.srcRect.y = sprite.height * 1;
						break;
					case SDLK_DOWN:
						rigidBody.velocity = keyboardControl.downVelocity;
						sprite.srcRect.y = sprite.height * 2;
						break;
					case SDLK_LEFT:
						rigidBody.velocity = keyboardControl.leftVelocity;
						sprite.srcRect.y = sprite.height * 3;
						break;
				}
			}
		}

		void Update() {

		}
};

#endif