#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "SDL.h"

#include "../ECS/ECS.h"
#include "../EventBus//Event.h"

class KeyPressedEvent : public Event {
    public:
        SDL_Keycode symbol;

        KeyPressedEvent(SDL_Keycode symbol) : symbol(symbol) {}

        ~KeyPressedEvent() = default;
};

#endif
