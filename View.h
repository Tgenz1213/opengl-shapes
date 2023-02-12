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
	bool createShaderProgram();
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

	// Ignore normals for now

	/* Vertex Shader Source Code */
	const GLchar* const vertexShaderSource = GLSL(440,
		layout(location = 0) in vec3 position;
	layout(location = 2) in vec2 texCoord;

	out vec2 vertexTextureCoordinate;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f);
		vertexTextureCoordinate = texCoord;
	}
	);

	/* Fragment Shader Source Code */
	const GLchar* const fragmentShaderSource = GLSL(440,
		in vec2 vertexTextureCoordinate;

	out vec4 fragmentColor;

	uniform sampler2D texture0;
	uniform sampler2D texture1;
	uniform int activeTexture;

	void main()
	{
		if (activeTexture == 0)
			fragmentColor = texture(texture0, vertexTextureCoordinate);
		else if (activeTexture == 1)
			fragmentColor = texture(texture1, vertexTextureCoordinate);
	}
	);
};
