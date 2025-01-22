#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus//Event.h"
#include "SDL.h"

class CollisionEvent: public Event {
    public:
        Entity entityA;
        Entity entityB;

        CollisionEvent(Entity entityA, Entity entityB)
            : entityA(entityA), entityB(entityB) {}

        ~CollisionEvent();
};

#endif
