
#ifndef HEADERS_DEFINITION_H_
#define HEADERS_DEFINITION_H_

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/random.hpp>
#include <iostream>

typedef struct _VertexColor {

	_VertexColor() {
	}

	_VertexColor(glm::vec3 position, glm::vec3 color) {
		this->position = position;
		this->color = color;
	}

	glm::vec3 position;
	glm::vec3 color;
} VertexColor;

typedef struct _VertexLightColor {

	_VertexLightColor() {
	}

	_VertexLightColor(glm::vec3 position, glm::vec3 color, glm::vec3 normal) {
		this->position = position;
		this->color = color;
		this->normal = normal;
	}

	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
} VertexLightColor;

typedef struct _VertexLightTexture {

	_VertexLightTexture() {
	}

	_VertexLightTexture(glm::vec3 position, glm::vec2 texture,
			glm::vec3 normal) {
		this->position = position;
		this->texture = texture;
		this->normal = normal;
	}

	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;
} VertexLightTexture;

typedef struct _VertexNormal {
	_VertexNormal() {
	}

	_VertexNormal(glm::vec3 position, glm::vec3 normal, glm::vec3 interPolatedNormal, glm::vec3 interPolatedPosition) {
		this->position = position;
		this->normal = normal;
		this->interPolatedNormal = interPolatedNormal;
		this->interPolatedPosition = interPolatedPosition;
	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 interPolatedNormal;
	glm::vec3 interPolatedPosition;

} VertexNormal;

enum MODEL_MODE {
	VERTEX,
	VERTEX_COLOR,
	VERTEX_TEXTURE,
	VERTEX_LIGHT_COLOR,
	VERTEX_LIGHT_TEXTURE
};

#endif /* HEADERS_DEFINITION_H_ */
