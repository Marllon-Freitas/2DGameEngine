#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include <SDL.h>

#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"

class KeyboardControlSystem : public System {
	public:
		KeyboardControlSystem() {}

		void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
			eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
		}

		void OnKeyPressed(KeyPressedEvent& event) {
			std::string keyCode = std::to_string(event.symbol);
			std::string keySymbol(1, event.symbol);
			Logger::Info("Key pressed event emitted: [" + keyCode + "] " + keySymbol);
		}

		void Update() {

		}
};

#endif