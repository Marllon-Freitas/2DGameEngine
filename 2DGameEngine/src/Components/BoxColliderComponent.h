#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <string>
#include <SDL.h>
#include <glm/glm.hpp>

struct BoxColliderComponent {
	int width;
	int height;
	glm::vec2 offSet;

	BoxColliderComponent(
		int width = 0,
		int height = 0,
		glm::vec2 offSet = glm::vec2(0)
	) {
		this->width = width;
		this->height = height;
		this->offSet = offSet;
	}
};

#endif
