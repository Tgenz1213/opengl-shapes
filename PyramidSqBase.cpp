#include "PyramidSqBase.h"

void PyramidSqBase::createVertices()
{
	std::vector<GLfloat> vertices = {
		// front face
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		// right face
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		// back face
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		// left face
		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		// base
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f
	};

	setVertices(vertices);
}

void PyramidSqBase::createNormals()
{
	std::vector<GLfloat> normals = {
		// front face
		0.0f, 0.4472f, 0.8944f,
		0.0f, 0.4472f, 0.8944f,
		0.0f, 0.4472f, 0.8944f,
		// right face
		0.8944f, 0.4472f, 0.0f,
		0.8944f, 0.4472f, 0.0f,
		0.8944f, 0.4472f, 0.0f,
		// back face
		0.0f, 0.4472f, -0.8944f,
		0.0f, 0.4472f, -0.8944f,
		0.0f, 0.4472f, -0.8944f,
		// left face
		-0.8944f, 0.4472f, 0.0f,
		-0.8944f, 0.4472f, 0.0f,
		-0.8944f, 0.4472f, 0.0f,
		// base
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};

	setNormals(normals);
}

void PyramidSqBase::createTextureCoordinates()
{
	std::vector<GLfloat> texCoords = {
		// front face
		0.5f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		// right face
		0.5f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		// back face
		0.5f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		// left face
		0.5f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		// base
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	setTextureCoordinates(texCoords);
}

void PyramidSqBase::createIndices()
{
	std::vector<GLuint> indices = {
		// front face
		0, 1, 2,
		// right face
		3, 4, 5,
		// back face
		6, 7, 8,
		// left face
		9, 10, 11,
		// base
		12, 13, 14,
		12, 14, 15
	};

	setIndices(indices);
}