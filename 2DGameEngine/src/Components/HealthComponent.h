#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <string>
#include <SDL.h>
#include <glm/glm.hpp>

struct HealthComponent {
	int healthPercentage;

	HealthComponent(int healthPercentage = 0) {
		this->healthPercentage = healthPercentage;
	}
};

#endif
