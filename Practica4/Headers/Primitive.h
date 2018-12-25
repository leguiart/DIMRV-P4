#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>
#include "definition.h"


class Primitive {
public:
	Primitive(){}
	Primitive(MODEL_MODE mode) : mode(mode) {}
	Primitive(float ratio, int slices, int stacks, MODEL_MODE mode) : ratio(ratio), slices(slices), stacks(stacks), mode(mode) {}
	Primitive(float ratio, float height, int slices, MODEL_MODE mode) : ratio(ratio), height(height), slices(slices), mode(mode) {}
	Primitive(int slices, int stacks, glm::vec3 color, MODEL_MODE mode) : slices(slices), stacks(stacks),  color(color), mode(mode) {}
protected:
	std::vector<VertexColor> vertexC;
	std::vector<VertexLightColor> vertexLC;
	std::vector<VertexLightTexture> vertexLT;
	std::vector<GLuint> index;
	MODEL_MODE mode;
	float ratio, height;
	int slices;
	int stacks;
	glm::vec3 color;
	GLuint VAO, VBO, EBO;
};

class Sphere : public Primitive {
public:
	Sphere(float ratio, int slices, int stacks, MODEL_MODE mode) : Primitive(ratio, slices, stacks, mode) {}
	virtual ~Sphere();
	void init();
	void load();
	void render();
	glm::vec3 searchVertex(glm::vec3 pos);
	glm::vec3 getVertexNormal(int index) { return vertexLC[index].normal; }

};

class Cylinder : public Primitive {
public:
	Cylinder(float ratio, float height, int slices, MODEL_MODE mode) : Primitive(ratio, height, slices, mode) {}
	virtual ~Cylinder();
	void init();
	void load();
	void render();
};

class Cube : public Primitive {
public:
	Cube() : Primitive(MODEL_MODE::VERTEX_COLOR) {}
	virtual ~Cube();
	void load();
	void render();
};

class Pyramid : public Primitive {
public:
	Pyramid() : Primitive(MODEL_MODE::VERTEX_COLOR) {}
	virtual ~Pyramid();
	void load();
	void render();
};

class Piso : public Primitive {
public:
	Piso(){}
	Piso(int slices, int stacks, glm::vec3 color, MODEL_MODE mode) : Primitive(slices, stacks, color, mode) {}
	virtual ~Piso();
	void init();
	void load();
	void render();
	//int searchVertex(glm::vec2 pos);
	//std::vector<VertexNormal> getVertexNormals() { return vertexNormal; }
	float getHeight() { return height; }

private:
	//std::vector<VertexNormal> vertexNormal;
};



