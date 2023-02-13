#pragma once

#include "framework.h"
#include "Shape.h"

/*Shader program Macro*/
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

class View
{
public:
	// Constructor and deconstructor
	View() { vCamera = Camera(cameraPosition); }
	~View() = default;

	// Methods
	bool createShaderProgram(const GLchar* const vertexShaderSource, const GLchar* const fragmentShaderSource);
	void destroyShaderProgram();
	void resizeWindow(int width, int height);

	// Wrappers
	static void resizeWindowWrapper(GLFWwindow* window, int width, int height);

	// Attributes
	const char* const WINDOW_TITLE = "Timothy Genz";
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	Camera vCamera;
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
	float vLastX = (float)WINDOW_WIDTH / 2.0f;
	float vLastY = (float)WINDOW_HEIGHT / 2.0f;
	bool vFirstMouse = true;

	GLuint vProgramId;
	GLFWwindow* vWindow = nullptr;

	bool isOrtho = false;	// For toggling between perspective and orthographic
};
