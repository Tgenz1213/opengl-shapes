#include "Source.h"

int main(int argc, char* argv[])
{
	// Initialize view and factory
	View mView;
	ShapeFactory mFactory;
	const char* textureFilenameWood = "C:/Users/Tim/source/repos/cs330/resources/woodgrain.jpg";
	const char* textureFilenameOrange = "C:/Users/Tim/source/repos/cs330/resources/orange.jpg";

	// Initialize controller
	auto mController = Controller(&mView);

	if (!mController.initialize(argc, argv))
		return EXIT_FAILURE;

	// Create shapes
	Cube mCubeSmall = mFactory.createCube();

	Cube mCubeLarge = mFactory.createCube();

	UCylinder mCylinder = mFactory.createCylinder();

	// Create the shader program
	if (!mView.createShaderProgram(ShaderTexOnly::vertexShaderSource, ShaderTexOnly::fragmentShaderSource))
		return EXIT_FAILURE;

	// Load wood texture
	glActiveTexture(GL_TEXTURE0);
	if (!ULoadTexture(textureFilenameWood, &mCubeSmall))
	{
		std::cout << "Failed to load cube texture " << mCubeSmall.texId << std::endl;
		return EXIT_FAILURE;
	}

	if (!ULoadTexture(textureFilenameWood, &mCubeLarge))
	{
		std::cout << "Failed to load cube texture " << mCubeLarge.texId << std::endl;
		return EXIT_FAILURE;
	}

	// Load orange texture
	glActiveTexture(GL_TEXTURE1);
	if (!ULoadTexture(textureFilenameOrange, &mCylinder))
	{
		std::cout << "Failed to load cylinder texture " << mCylinder.texId << std::endl;
		return EXIT_FAILURE;
	}

	glUseProgram(mView.vProgramId);

	glUniform1i(glGetUniformLocation(mView.vProgramId, "texture0"), 0);
	glUniform1i(glGetUniformLocation(mView.vProgramId, "texture1"), 1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// render loop
	// -----------
	// Main loop
	while (!glfwWindowShouldClose(mView.vWindow))
	{
		// per-frame timing
		// --------------------
		auto currentFrame = static_cast<float>(glfwGetTime());
		mController.cDeltaTime = currentFrame - mController.cLastFrame;
		mController.cLastFrame = currentFrame;

		// input
		// -----
		mController.processInput();

		// Enable z-depth
		glEnable(GL_DEPTH_TEST);

		// Clear the color buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(mView.vProgramId);

		// Chooses which texture to use
		GLint activeTextureLocation = glGetUniformLocation(mView.vProgramId, "activeTexture");

		// Bind cube texture
		glActiveTexture(GL_TEXTURE0);	// Woodgrain.jpg
		glBindTexture(GL_TEXTURE_2D, mCubeSmall.texId);
		glBindTexture(GL_TEXTURE_2D, mCubeLarge.texId);

		glActiveTexture(GL_TEXTURE1);	// Orange.jpg
		glBindTexture(GL_TEXTURE_2D, mCylinder.texId);

		// Draw cubes
		UTranslateShape(&mCubeSmall, glm::vec3(1.0f, 1.0f, 0.75f), glm::vec3(-0.3f, -0.5f, 0.0f), 0, &mView); // Retrieves and passes transform matrices to the Shader program
		glUniform1i(activeTextureLocation, 0);	// Set wood texture
		glBindVertexArray(mCubeSmall.vao);
		mCubeSmall.draw();
		glBindVertexArray(0);

		UTranslateShape(&mCubeLarge, glm::vec3(1.0f, 1.0f, 0.99f), glm::vec3(0.5f, -0.5f, 0.0f), 0, &mView);
		glBindVertexArray(mCubeLarge.vao);
		mCubeSmall.draw();
		glBindVertexArray(0);

		// Draw cylinder
		UTranslateShape(&mCylinder, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0, &mView);
		glUniform1i(activeTextureLocation, 1);	// Switch to orange texture
		glBindVertexArray(mCylinder.vao);
		mCylinder.draw();
		glBindVertexArray(0);

		// Set view mode. Variable changed with P key press
		if (mView.isOrtho)
			mController.setViewModeOrthographic();
		else
			mController.setViewModePerspective();

		// Swap buffers
		glfwSwapBuffers(mView.vWindow);

		// Handle input events
		glfwPollEvents();
	}

	// Release textures
	glGenTextures(1, &mCubeSmall.texId);
	glGenTextures(1, &mCubeLarge.texId);
	glGenTextures(1, &mCylinder.texId);

	// Release shader program
	mView.destroyShaderProgram();

	exit(EXIT_SUCCESS); // Terminates the program successfully
}

// Add texture to shape
bool ULoadTexture(const char* imagePath, Shape* shape)
{
	int width;
	int height;
	int channels;
	unsigned char* image = stbi_load(imagePath, &width, &height, &channels, 0);
	if (image)
	{
		UFlipImageVertically(image, width, height, channels);

		glGenTextures(1, &shape->texId);
		glBindTexture(GL_TEXTURE_2D, shape->texId);

		// Set the texture wrapping parameters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << channels << " channels" << std::endl;
			return false;
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture.

		return true;
	}

	// Error loading the image
	return false;
}

// Flip image to match OpenGL Y-coordinates
void UFlipImageVertically(unsigned char* image, int width, int height, int channels)
{
	for (int j = 0; j < height / 2; ++j)
	{
		int index1 = j * width * channels;
		int index2 = (height - 1 - j) * width * channels;

		if (index1 >= height * width * channels || index2 >= height * width * channels)
		{
			std::cout << "Image flip failed. Width, height, or channels are incorrect for " << image << std::endl;
			break;
		}

		for (int i = width * channels; i > 0; --i)
		{
			unsigned char tmp = image[index1];
			image[index1] = image[index2];
			image[index2] = tmp;
			++index1;
			++index2;
		}
	}
}

// Modifies a shape to rotate, scale, and translate
void UTranslateShape(Shape* shape, glm::vec3 scale, glm::vec3 translation, float rotation, const View* view)
{
	// 1. Scales the object
	glm::mat4 scaleMatrix = glm::scale(scale);
	// 2. Rotates shape in the x axis
	glm::mat4 rotationMatrix = glm::rotate(rotation, glm::vec3(1.0f, 1.0f, 1.0f));
	// 3. Place object from the origin
	glm::mat4 translationMatrix = glm::translate(translation);
	// Model matrix: transformations are applied right-to-left order
	shape->model = scaleMatrix * rotationMatrix * translationMatrix;

	// Retrieves and passes transform matrices to the Shader program
	GLint modelLoc = glGetUniformLocation(view->vProgramId, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(shape->model));
}