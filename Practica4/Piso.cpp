#include "Headers/Primitive.h"

Piso::~Piso() {

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDeleteBuffers(1, &EBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

}

void Piso::init() {
	if(mode == MODEL_MODE::VERTEX_COLOR)
		vertexC.resize(((slices + 1) * (stacks + 1)));
	if(mode == MODEL_MODE::VERTEX_LIGHT_COLOR)
		vertexLC.resize(((slices + 1) * (stacks + 1)));
	index.resize((slices * stacks + slices) * 6);
	for (int i = 0; i <= stacks; ++i) { //Meridianos
		float V = i / (float)stacks;
		for (int j = 0; j <= slices; ++j) { //Paralelos
			float U = j / (float)slices;
			if (mode == MODEL_MODE::VERTEX_COLOR) {
				vertexC[i * (slices + 1) + j].position = glm::vec3(U, -0.5, V);
				vertexC[i * (slices + 1) + j].color = color;
			}
			else if (mode == MODEL_MODE::VERTEX_LIGHT_COLOR) {
				vertexLC[i * (slices + 1) + j].position = glm::vec3(U, -0.5, V);
				vertexLC[i * (slices + 1) + j].color = color;
				vertexLC[i * (slices + 1) + j].normal = glm::vec3(U, 1.0f, V);
			}
			
			//std::cout << "X: " << X << " , Y: " << Y << ", Z:" << Z << std::endl;
			//std::cout << "U: " << U << " , " << "V: " << V<<std::endl;
		}
	}

	for (int i = 0; i < slices * stacks + slices; ++i) {
		index[i * 6] = i;
		index[i * 6 + 1] = i + slices + 1;
		index[i * 6 + 2] = i + slices;
		index[i * 6 + 3] = i + slices + 1;
		index[i * 6 + 4] = i;
		index[i * 6 + 5] = i + 1;
	}
}

void Piso::load() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	size_t stride;

	size_t offset1 = 0;
	size_t offset2 = 0;
	size_t offset3 = 0;

	if (mode == MODEL_MODE::VERTEX_COLOR) {
		glBufferData(GL_ARRAY_BUFFER, vertexC.size() * sizeof(glm::vec3) * 2,
			vertexC.data(),
			GL_STATIC_DRAW);
		stride = sizeof(vertexC[0]);
		offset1 = 0;
	}
	else if (mode == MODEL_MODE::VERTEX_LIGHT_COLOR) {
		glBufferData(GL_ARRAY_BUFFER, vertexLC.size() * sizeof(glm::vec3) * 3,
			vertexLC.data(),
			GL_STATIC_DRAW);
		stride = sizeof(vertexLC[0]);
		offset1 = 0;
		offset2 = sizeof(vertexLC[0].position);
		offset3 = sizeof(vertexLC[0].position) + sizeof(vertexLC[0].color);
	}else if (mode == MODEL_MODE::VERTEX_LIGHT_TEXTURE) {
		glBufferData(GL_ARRAY_BUFFER,
			vertexLT.size() * (sizeof(glm::vec3) * 2 + sizeof(glm::vec2)),
			vertexLT.data(),
			GL_STATIC_DRAW);
		stride = sizeof(vertexLT[0]);
		offset1 = 0;
		offset2 = sizeof(vertexLT[0].position);
		offset3 = sizeof(vertexLT[0].position) + sizeof(vertexLT[0].texture);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
		index.data(),
		GL_STATIC_DRAW);

	// First attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset1);
	glEnableVertexAttribArray(0);
	// Second attribute
	if (mode == MODEL_MODE::VERTEX_LIGHT_TEXTURE)
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride,
		(GLvoid*)offset2);
	else
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
		(GLvoid*)offset2);
	glEnableVertexAttribArray(1);
	// Thrid attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)offset3);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

}

void Piso::render() {

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT,
		(GLvoid*)(sizeof(GLuint) * 0));
	glBindVertexArray(0);

}




