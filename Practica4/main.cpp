//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/InputManager.h"
#include "Headers/TimeManager.h"
#include "Headers/Shader.h"
#include "Headers/Primitive.h."

using namespace std;
using namespace glm;

GLuint VBO, VAO;
Shader shader;
int screenWidth;
int screenHeight;

double deltaTime, elapsedTime;
GLFWwindow * window;
InputManager inputManager1 = InputManager(vec3(20.0f, 50.0f, 50.0),  vec3(0.0, 1.0, 0.0), GOD_MODE);
InputManager inputManager2 = InputManager( vec3(0.0f, 2.0f, 40.0),  vec3(0.0, 1.0, 0.0), TERRAIN);
InputManager inputManager3 = InputManager( vec3(0.0f, 50.0f, 40.0),  vec3(-1.0f, 0.0f, 0.0f), TOP_DOWN);

bool cam_top = false, cam_terr = true, cam_god = false;
 vec3 color;
//Sphere sp = Sphere(40.0, 40, 40, MODEL_MODE::VERTEX_LIGHT_COLOR);
Piso piso, piso1, piso2;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroyWindow();
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroyWindow();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	shader.initialize("../Shaders/mvpColor.vs", "../Shaders/mvpColor.fs");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	

	piso = Piso(30, 30, vec3(1.0, 0.0, 0.0), VERTEX_LIGHT_COLOR);
	piso1 = Piso(30, 30, vec3(0.0, 0.0, 0.2), VERTEX_LIGHT_COLOR);
	piso2 = Piso(30, 30, vec3(0.3, 0.0, 0.0), VERTEX_LIGHT_COLOR);
	piso.init();
	piso.load();
	piso1.init();
	piso1.load();
	piso2.init();
	piso2.load();
	inputManager3.getCamera()->setFrontTopDown(vec3(0.0f, -1.0f, 0.0));
	inputManager2.getCamera()->setUpTerrain(vec3(0.0f, 1.0f, 0.0));
}
void destroyWindow() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void destroy() {
	destroyWindow();
	shader.destroy();
	piso.~Piso();
	piso1.~Piso();
	piso2.~Piso();
	
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VBO);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if(cam_god)
		inputManager1.keyPressed(inputManager1.toApplicationKey(key), deltaTime, inputManager1.toApplicationState(action));

	if(cam_terr)
		inputManager2.keyPressed(inputManager2.toApplicationKey(key), deltaTime, inputManager2.toApplicationState(action));

	if(cam_top)
		inputManager3.keyPressed(inputManager3.toApplicationKey(key), deltaTime, inputManager3.toApplicationState(action));

	
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		cam_top = true; cam_god = false; cam_terr = false;
	}
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
	{
		cam_terr = true; cam_god = false; cam_top = false;
	}
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
	{
		cam_god = true; cam_terr = false; cam_top = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if(cam_god)
		inputManager1.mouseMoved(xpos, ypos);
	if(cam_terr)
		inputManager2.mouseMoved(xpos, ypos);
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if(cam_god)
		inputManager1.mouseClicked(inputManager1.toMouseButtonIndex(button), xpos, ypos, inputManager1.toApplicationState(state));
	if(cam_terr)
		inputManager2.mouseClicked(inputManager2.toMouseButtonIndex(button), xpos, ypos, inputManager2.toApplicationState(state));
}

void mouseScrollCallback(GLFWwindow* window, double xoffset,
	double yoffset) {
	inputManager3.mouseScroll(yoffset);
}

bool processInput(bool continueApplication) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS
		|| glfwWindowShouldClose(window) != 0) {
		return false;
	}
	TimeManager::Singleton().CalculateFrameRate(false);
	deltaTime = TimeManager::Singleton().DeltaTime;
	glfwPollEvents();

	if(cam_god)
		inputManager1.do_movement(deltaTime);

	if (cam_terr)
	{
		inputManager2.do_movement(deltaTime);
		if (inputManager2.getCamera()->Position.z >= 0.0f && inputManager2.getCamera()->Position.z <= 1.0f) {
			inputManager2.getCamera()->setUpTerrain( vec3(0.0f, 0.85355f, 0.353554f));
		}
		else if (inputManager2.getCamera()->Position.z < 0.0f && inputManager2.getCamera()->Position.z > -34.0f) {
			inputManager2.getCamera()->setUpTerrain( vec3(0.0f, 0.707107f, 0.707107f));
		}
		else if (inputManager2.getCamera()->Position.z > 1.5f || inputManager2.getCamera()->Position.z < -35.5f) {
			inputManager2.getCamera()->setUpTerrain( vec3(0.0f, 1.0f, 0.0f));
		}
		else if (inputManager2.getCamera()->Position.z <= -34.0f && inputManager2.getCamera()->Position.z > -35.5f) {
			inputManager2.getCamera()->setUpTerrain( vec3(0.0f, 0.85355f, 0.353554f));
		}
	}
	
	if (cam_top) {
		inputManager3.do_movement(deltaTime);
		if (inputManager3.getCamera()->Position.z >= -1.0f && inputManager3.getCamera()->Position.z <= 1.0f) {
			inputManager3.getCamera()->setFrontTopDown( vec3(0.0f, -0.85355f, -0.353554f));
		}
		else if (inputManager3.getCamera()->Position.z < -1.0f && inputManager3.getCamera()->Position.z > -34.0f) {
			inputManager3.getCamera()->setFrontTopDown( vec3(0.0f, -0.707107f, -0.707107f));
		}
		else if (inputManager3.getCamera()->Position.z > 1.5f || inputManager3.getCamera()->Position.z < -35.5f) {
			inputManager3.getCamera()->setFrontTopDown( vec3(0.0f, -1.0f, 0.0f));
		}
		else if (inputManager3.getCamera()->Position.z <= -20.0f && inputManager3.getCamera()->Position.z > -35.5f) {
			inputManager3.getCamera()->setFrontTopDown( vec3(0.0f, -0.85355f, -0.353554f));
		}
	}

	cout << inputManager3.getCamera ()->Position.z << endl;
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	double lastTime = TimeManager::Singleton().GetTime();
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	 mat4 projection;
	projection =  perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);	
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		elapsedTime += deltaTime;

		shader.turnOn();
		// Create transformations
		mat4 view;
		mat4 model;
		if(cam_god)
			view = inputManager1.getCamera()->GetViewMatrix();
		if(cam_terr)
			view = inputManager2.getCamera()->GetViewMatrix();
		if(cam_top)
			view = inputManager3.getCamera()->GetViewMatrix();
		// Get their uniform location
		GLint modelLoc = shader.getUniformLocation("model");
		GLint viewLoc = shader.getUniformLocation("view");
		GLint projLoc = shader.getUniformLocation("projection");
		//model =  rotate(model, -(float)M_PI,  vec3(1.0f, 0.0f, 0.0f))*  rotate(model, -(float)M_PI*0.5f,  vec3(0.0f, 1.0f, 0.0f));
		//glDisable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);
		model =  translate( vec3(-100.0f, 0.5f, -100.0f))* scale( vec3(200.0f, 1.0f, 200.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE,  value_ptr(model));
		piso.render();
		
		model =  mat4();
		model =  rotate( mat4(), (float)M_PI / 4.0f,  vec3(1.0, 0.0, 0.0))* translate( vec3(-100.0f, 0.5f, -50.0f))* scale( vec3(200.0f, 1.0f, 50.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE,  value_ptr(model));
		piso1.render();

		model =  mat4();
		model =  translate( vec3(-100.0f, 36.35533f, -100.0f))* scale( vec3(200.0f, 1.0f, 65.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE,  value_ptr(model));
		piso2.render();
		//color =  vec3( cos(timeValue * 5 + deltaTime) + 0.2,  sin(timeValue * 10) + 0.2,  sin(timeValue * 5 + deltaTime) + 0.2);
		
		// Pass the matrices to the shader
		//model =  mat4();
		//model =  rotate( toMat4(inputManager.rotacion), rot,  vec3(1.0, 0.5, 0.3));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE,  value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE,  value_ptr(projection));
		//glUniform3fv(shader.getUniformLocation("color"), GL_TRUE,  value_ptr(color));
		//sp.render();
		
		shader.turnOff();

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}


