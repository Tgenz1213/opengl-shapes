#include "View.h"
#include "Controller.h"

// Create shader
bool View::createShaderProgram()
{
	// Compilation and linkage error reporting
	int success = 0;
	char infoLog[512];

	// Create a Shader program object.
	vProgramId = glCreateProgram();

	// Create the vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// Retrive the shader source
	glShaderSource(vertexShaderId, 1, &vertexShaderSource, nullptr);
	glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, nullptr);

	// Compile the vertex shader, and print compilation errors (if any)
	glCompileShader(vertexShaderId); // compile the vertex shader
	// check for shader compile errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glCompileShader(fragmentShaderId); // compile the fragment shader
	// check for shader compile errors
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	// Attached compiled shaders to the shader program
	glAttachShader(vProgramId, vertexShaderId);
	glAttachShader(vProgramId, fragmentShaderId);

	glLinkProgram(vProgramId);   // links the shader program
	// check for linking errors
	glGetProgramiv(vProgramId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(vProgramId, sizeof(infoLog), nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glUseProgram(vProgramId);    // Uses the shader program

	return true;
}

void View::destroyShaderProgram()
{
	glDeleteProgram(vProgramId);
}

void View::resizeWindow(int width, int height)
{
	glViewport(0, 0, width, height);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("View::resizeWindow - OpenGL error: %d\n", error);
	}
}

void View::resizeWindowWrapper(GLFWwindow* window, int width, int height)
{
	View* view = static_cast<View*>(glfwGetWindowUserPointer(window));
	view->resizeWindow(width, height);
}