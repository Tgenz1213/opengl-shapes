#include "View.h"

// Create shader
bool View::createShaderProgram(const GLchar* const vertexShaderSource, const GLchar* const fragmentShaderSource)
{
	// Compilation and linkage error reporting
	int success = 0;
	char infoLog[512];

	// Create a Shader program object.
	currentProgramId = glCreateProgram();

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
	glAttachShader(currentProgramId, vertexShaderId);
	glAttachShader(currentProgramId, fragmentShaderId);

	glLinkProgram(currentProgramId);   // links the shader program
	// check for linking errors
	glGetProgramiv(currentProgramId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(currentProgramId, sizeof(infoLog), nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;

		return false;
	}

	glUseProgram(currentProgramId);    // Uses the shader program

	return true;
}

void View::destroyShaderProgram(GLuint programId) const
{
	glDeleteProgram(programId);
}

void View::resizeWindow(int width, int height) const
{
	glViewport(0, 0, width, height);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("View::resizeWindow - OpenGL error: %d\n", error);
	}
}

// Camera speed 2.5f to 2.5f*3
void View::setCameraSpeed(float speed)
{
	static const float cameraSpeed = 2.5f;

	vCamera.MovementSpeed += cameraSpeed * speed;
	if (vCamera.MovementSpeed < cameraSpeed)
		vCamera.MovementSpeed = cameraSpeed;
	if (vCamera.MovementSpeed > cameraSpeed * 3.0f)
		vCamera.MovementSpeed = cameraSpeed * 3.0f;
}

// Draw shapes with user assigned textures
void View::drawShape(const Shape& shape) const
{
	GLint activeTextureLocation = glGetUniformLocation(currentProgramId, "uTexture");

	glActiveTexture(GL_TEXTURE0 + shape.texId);	// Woodgrain.jpg
	glBindTexture(GL_TEXTURE_2D, shape.texId);

	glUniform1i(activeTextureLocation, shape.texId);

	glBindVertexArray(shape.vao);
	shape.draw();
	glBindVertexArray(0);
}

void View::setViewModePerspective() const
{
	// Transforms the camera: move the camera back (z axis)
	glm::mat4 view = vCamera.GetViewMatrix();

	glm::mat4 projection = glm::perspective(glm::radians(vCamera.Zoom), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to the Shader program
	GLint viewLoc = glGetUniformLocation(currentProgramId, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLint projLoc = glGetUniformLocation(currentProgramId, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void View::setViewModeOrthographic() const
{
	// Transforms the camera: move the camera back (z axis)
	glm::mat4 view = vCamera.GetViewMatrix();

	glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to the Shader program
	GLint viewLoc = glGetUniformLocation(currentProgramId, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	GLint projLoc = glGetUniformLocation(currentProgramId, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void View::useProgram(GLuint programId)
{
	setProgramId(programId);
	glUseProgram(currentProgramId);
}

void View::resizeWindowWrapper(GLFWwindow* window, int width, int height)
{
	auto const* view = static_cast<View*>(glfwGetWindowUserPointer(window));
	view->resizeWindow(width, height);
}