#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "SDL.h"

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class CollisionEvent: public Event {
    public:
        Entity entityA;
        Entity entityB;

        CollisionEvent(Entity entityA, Entity entityB)
            : entityA(entityA), entityB(entityB) {}

        ~CollisionEvent() = default;
};

#endif
