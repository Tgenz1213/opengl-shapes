#include "Plane.h"

void Plane::createVertices()
{
	std::vector<GLfloat> vertices = {
		-1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, -1.0f
	};

	setVertices(vertices);
}

void Plane::createNormals()
{
	std::vector<GLfloat> normals = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	setNormals(normals);
}

void Plane::createTextureCoordinates()
{
	std::vector<GLfloat> texCoords = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	setTextureCoordinates(texCoords);
}

void Plane::createIndices()
{
	std::vector<GLuint> indices = {
		0, 1, 2,
		2, 3, 0
	};

	setIndices(indices);
}