#include "Cylinder.h"
#include <cmath>

// Constructor with arguments
Cylinder::Cylinder(GLfloat baseRadius, GLfloat topRadius, GLfloat height, GLuint nSides, GLuint nStacks)
{
	setBaseRadius(baseRadius);
	setTopRadius(topRadius);
	setHeight(height);
	setSides(nSides);
	setStacks(nStacks);
	bindVertexArrayAndBuffers();
}

// Method to create vertices
void Cylinder::createVertices() {
	GLfloat step = 2 * glm::pi<GLfloat>() / (GLfloat)nSides;
	GLfloat unitHeight = height / (GLfloat)nStacks;
	GLfloat unitRadius = (topRadius - baseRadius) / (GLfloat)nStacks;

	// Generate vertices for bottom face
	for (GLuint i = 0; i <= nSides; ++i) {
		GLfloat x = baseRadius * cos((GLfloat)i * step);
		GLfloat z = baseRadius * sin((GLfloat)i * step);
		GLfloat y = -height / 2.0f;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}

	// Generate vertices for sides
	for (GLuint i = 0; i <= nStacks; ++i) {
		GLfloat radius = baseRadius + (GLfloat)i * unitRadius;
		GLfloat y = -height / 2.0f + (GLfloat)i * unitHeight;
		for (GLuint j = 0; j < nSides; ++j) {
			GLfloat x = radius * cos((GLfloat)j * step);
			GLfloat z = radius * sin((GLfloat)j * step);
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	// Generate vertices for top face
	for (GLuint i = 0; i <= nSides; ++i) {
		GLfloat x = topRadius * cos((GLfloat)i * step);
		GLfloat z = topRadius * sin((GLfloat)i * step);
		GLfloat y = height / 2.0f;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}
}

// Generate normals. Top face and last stack are interfering with eachother. Not sure why.
void Cylinder::createNormals() {
	// Bottom face
	for (GLuint i = 0; i <= nSides; ++i) {
		// Add normal for bottom vertex
		normals.push_back(0.0f);
		normals.push_back(-1.0f);
		normals.push_back(0.0f);
	}

	for (GLuint i = 0; i <= nStacks; ++i) {
		for (GLuint j = 0; j < nSides; ++j) {
			// Calculate the index of the current vertex in the vertices array
			GLuint vertexIndex = i * nSides + j;
			// Calculate the position of the current vertex
			glm::vec3 pos(vertices[vertexIndex * 3], vertices[vertexIndex * 3 + 1], vertices[vertexIndex * 3 + 2]);
			// Calculate the normal for the current vertex
			glm::vec3 normal = glm::normalize(glm::vec3(pos.x, 0.0f, pos.z));
			// Add the normal to the normals array
			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
		}
	}

	// Top face
	for (GLuint i = 0; i <= nSides; ++i) {
		// Add normal for top vertex
		normals.push_back(0.0f);
		normals.push_back(1.0f);
		normals.push_back(0.0f);
	}
}

// Generate texture coordinates
void Cylinder::createTextureCoordinates() {
	GLfloat step = 2 * glm::pi<GLfloat>() / (GLfloat)nSides;	// Calculate step angle
	GLfloat unitHeight = height / (GLfloat)nStacks;	// Calculate unit height

	// Bottom face
	for (GLuint i = 0; i <= nSides; ++i) {
		// Add texture coordinates for bottom vertex
		texCoords.push_back(0.5f + 0.5f * cos((GLfloat)i * step));
		texCoords.push_back(0.5f + 0.5f * sin((GLfloat)i * step));
	}

	// Calculate texture coordinates for sides
	for (GLuint i = 0; i <= nStacks; ++i) {
		GLfloat y = (GLfloat)i / (GLfloat)nStacks;
		for (GLuint j = 0; j < nSides; ++j) {
			GLuint vertexIndex = i * nSides + j;
			GLfloat x = atan2(vertices[vertexIndex * 3 + 2], vertices[vertexIndex * 3]) / (2 * glm::pi<GLfloat>());
			texCoords.push_back(x);
			texCoords.push_back(y);
		}
	}

	// Top face
	for (GLuint i = 0; i <= nSides; ++i) {
		// Add texture coordinates for top vertex
		texCoords.push_back(0.5f + 0.5f * cos((GLfloat)i * step));
		texCoords.push_back(0.5f + 0.5f * sin((GLfloat)i * step));
	}
}

// Generate indices
void Cylinder::createIndices() {
	// Generate indices for bottom face
	for (GLuint i = 0; i <= nSides; ++i) {
		if (i < nSides)
		{
			indices.push_back(0);
			indices.push_back(i + 1);
			indices.push_back(i + 2);
		}
		else
		{
			indices.push_back(0);
			indices.push_back(nSides);
			indices.push_back(1);
		}
	}

	// Something is going wrong between these two loops but I can't figure out what

	// Generate indices for sides
	for (GLuint i = 0; i <= nStacks; ++i) {
		for (GLuint j = 0; j < nSides; ++j) {
			GLuint k = (j + 1) % nSides;

			indices.push_back(i * nSides + j);
			indices.push_back(i * nSides + k);
			indices.push_back(i * nSides + j + nSides);

			indices.push_back(i * nSides + j + nSides);
			indices.push_back(i * nSides + k);
			indices.push_back(i * nSides + k + nSides);
		}
	}

	// Generate indices for top face
	auto nVertices = static_cast<GLuint>(vertices.size() / 3);
	GLuint topVertex = nVertices - nSides;
	for (GLuint i = 0; i <= nSides; ++i) {
		indices.push_back(topVertex + i);
		indices.push_back(nVertices - 1);
		indices.push_back(topVertex + i + 1);
	}
}