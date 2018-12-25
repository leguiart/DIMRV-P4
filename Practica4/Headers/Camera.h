
#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Primitive.h"

enum Camera_Movement {
	C_FORWARD, C_BACKWARD, C_LEFT, C_RIGHT
};

enum Camera_Mode {
	GOD_MODE, TOP_DOWN, TERRAIN
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVTY = 0.25f;
const float ZOOM = 45.0f;

class Camera {
public:
	// Camera Attributes
	glm::vec3 Position, Front, Up, Right, WorldUp, FrontSpeed, RightSpeed, UpSpeed, FrontTerrain, RightTerrain, UpTerrain;

	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	std::vector<VertexNormal> vertexNormal;

	// Constructor with vectors
	Camera(/*Sphere& sphere,*/ Camera_Mode mode, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up =
			glm::vec3(0.0f, 1.0f, 0.0f),float yaw = YAW, float pitch = PITCH) :
			Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(
					SENSITIVTY), Zoom(ZOOM) {
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
		this->mode = mode;
		if (this->mode == TOP_DOWN) {
			this->Up = glm::vec3(-1.0f, 0.0f, 0.0f);
			this->Front = glm::vec3(0.0f, -1.0f, 0.0f);
			this->Right = glm::vec3(0.0f, 0.0f, -1.0f);
		}
		//this->sphere = &sphere;
	}

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY,
			float upZ, float yaw, float pitch) :
			Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(
					SENSITIVTY), Zoom(ZOOM) {
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(this->Position, this->Position + this->Front,
				this->Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = this->MovementSpeed * deltaTime;
		if (this->mode == TERRAIN) {
			this->FrontSpeed = this->FrontTerrain * velocity;
			this->RightSpeed = this->RightTerrain * velocity;
		}
		if (this->mode == GOD_MODE) {
			this->FrontSpeed = this->Front * velocity;
			this->RightSpeed = this->Right * velocity;
		}
		if (this->mode == TOP_DOWN) {
			this->UpSpeed = this->Up * velocity;
			this->RightSpeed = this->Right * velocity;
			if (direction == C_FORWARD)
				this->Position += this->UpSpeed;
			if (direction == C_BACKWARD)
				this->Position -= this->UpSpeed;
			if (direction == C_LEFT)
				this->Position -= this->RightSpeed;
			if (direction == C_RIGHT)
				this->Position += this->RightSpeed;
		}
		if (this->mode == TERRAIN || this->mode == GOD_MODE) {
			if (direction == C_FORWARD)
				this->Position += this->FrontSpeed;
			if (direction == C_BACKWARD)
				this->Position -= this->FrontSpeed;
			if (direction == C_LEFT)
				this->Position -= this->RightSpeed;
			if (direction == C_RIGHT)
				this->Position += this->RightSpeed;
		}

	}

	void ProcessMouseMovement(float xoffset, float yoffset,
			GLboolean constrainPitch = true) {
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		if (constrainPitch) {
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		this->updateCameraVectors();
	}

	void setUpTerrain(glm::vec3 UpTerrain) {
		this->UpTerrain = UpTerrain;
		this->updateCameraVectors();
	}

	void setFrontTopDown(glm::vec3 Front) {
		this->Front = Front;
		this->updateCameraVectors();
	}

	void setCameraMode(Camera_Mode mode) {
		this->mode = mode;
	}

	Camera_Mode getCameraMode() {
		return mode;
	}

private:
	void updateCameraVectors() {
		glm::vec3 front;
		if (this->mode == TERRAIN || this->mode == GOD_MODE) {
			front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
			front.y = sin(glm::radians(this->Pitch));
			front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
			this->Front = glm::normalize(front);
			this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
			this->Up = glm::normalize(glm::cross(this->Right, this->Front));
		}

		if (this->mode == TERRAIN) {
			this->RightTerrain = glm::normalize(glm::cross(this->Front, this->UpTerrain));
			this->FrontTerrain = glm::normalize(glm::cross(this->UpTerrain, this->RightTerrain));
		}

		if (this->mode == TOP_DOWN) {
			this->Front = glm::normalize(this->Front);
			this->Right = glm::normalize(glm::cross(this->Front, this->Up));
		}

	}

	Sphere* sphere;
	Camera_Mode mode;
};
