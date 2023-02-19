#include "Cylinder.h"
#include <cmath>

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
	GLfloat step = 2 * glm::pi<GLfloat>() / (GLfloat)nSides;             // Calculate step angle
	GLfloat unitHeight = height / (GLfloat)nStacks;                      // Calculate unit height
	GLfloat unitRadius = (topRadius - baseRadius) / (GLfloat)nStacks;    // Calculate unit radius increment

	// Generate vertices for bottom face
	for (GLuint i = 0; i < nSides; ++i) {
		GLfloat x = baseRadius * cos((GLfloat)i * step);
		GLfloat z = baseRadius * sin((GLfloat)i * step);
		GLfloat y = -height / 2.0f;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}

	// Generate vertices for sides
	for (GLuint i = 0; i < nStacks; ++i) {
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
	for (GLuint i = 0; i < nSides; ++i) {
		GLfloat x = topRadius * cos((GLfloat)i * step);
		GLfloat z = topRadius * sin((GLfloat)i * step);
		GLfloat y = height / 2.0f;
		vertices.push_back(x);
		vertices.push_back(y);
		vertices.push_back(z);
	}
}

// Method to create normals
void Cylinder::createNormals() {
	GLfloat step = 2 * glm::pi<GLfloat>() / (GLfloat)nSides; // Calculate step angle
	GLfloat unitHeight = height / (GLfloat)nStacks; // Calculate unit height
	GLfloat unitRadius = (topRadius - baseRadius) / (GLfloat)nStacks; // Calculate unit radius increment

	// Generate normals for bottom face
	for (GLuint i = 0; i < nSides; ++i) {
		GLfloat x = baseRadius * cos((GLfloat)i * step);
		GLfloat z = baseRadius * sin((GLfloat)i * step);
		GLfloat y = -height / 2.0f;
		glm::vec3 dir = glm::normalize(glm::vec3(x, y, z));
		normals.push_back(dir.x);
		normals.push_back(dir.y);
		normals.push_back(dir.z);
	}

	// Generate normals for sides
	for (GLuint i = 0; i < nStacks; ++i) {
		GLfloat radius = baseRadius + (GLfloat)i * unitRadius;
		GLfloat y = -height / 2.0f + (GLfloat)i * unitHeight;
		for (GLuint j = 0; j < nSides; ++j) {
			GLfloat x = radius * cos((GLfloat)j * step);
			GLfloat z = radius * sin((GLfloat)j * step);
			glm::vec3 dir = glm::normalize(glm::vec3(x, y, z));
			normals.push_back(dir.x);
			normals.push_back(dir.y);
			normals.push_back(dir.z);
		}
	}

	// Generate normals for top face
	GLfloat y = 1.0f;
	for (GLuint i = 0; i < nSides; ++i) {
		GLfloat nx = topRadius * cos((GLfloat)i * step);
		GLfloat nz = topRadius * sin((GLfloat)i * step);
		glm::vec3 dir = glm::normalize(glm::vec3(nx, y, nz));
		normals.push_back(dir.x);
		normals.push_back(dir.y);
		normals.push_back(dir.z);
	}
}

void Cylinder::createTextureCoordinates() {
	GLfloat step = 2 * glm::pi<GLfloat>() / (GLfloat)nSides;             // Calculate step angle
	GLfloat unitHeight = height / (GLfloat)nStacks;                      // Calculate unit height

	// Calculate texture coordinates for bottom face
	GLfloat y = 0.0f;
	for (GLuint i = 0; i < nSides; ++i) {
		GLfloat x = 0.5f + 0.5f * cos((GLfloat)i * step);
		GLfloat z = 0.5f - 0.5f * sin((GLfloat)i * step);
		texCoords.push_back(x);
		texCoords.push_back(z);
	}

	// Calculate texture coordinates for sides
	for (GLuint i = 0; i < nStacks; ++i) {
		y = (GLfloat)i * unitHeight / height;
		for (GLuint j = 0; j < nSides; ++j) {
			GLfloat x = (GLfloat)j / (GLfloat)nSides;
			texCoords.push_back(x);
			texCoords.push_back(y);
		}
	}

	// Calculate texture coordinates for top face
	for (GLuint i = 0; i < nSides; ++i) {
		GLfloat x = 0.5f + 0.5f * cos((GLfloat)i * step);
		GLfloat z = 0.5f + 0.5f * sin((GLfloat)i * step);
		texCoords.push_back(x);
		texCoords.push_back(z);
	}
}

void Cylinder::createIndices() {
	auto nVertices = static_cast<GLuint>(vertices.size() / 3);
	GLuint nSideVerts = nSides + 1;

	// Generate indices for bottom face
	for (GLuint i = 0; i < nSides; ++i) {
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(nSideVerts);
	}

	// Generate indices for sides
	for (GLuint i = 0; i <= nStacks; i++) {
		for (GLuint j = 0; j < nSides; j++) {
			GLuint p0 = i * nSides + j;
			GLuint p1 = (i + 1) % nSides * nSides + j;
			GLuint p2 = i * nSides + (j + 1) % nSides;
			GLuint p3 = (i + 1) % nSides * nSides + (j + 1) % nSides;

			indices.push_back(p0);
			indices.push_back(p2);
			indices.push_back(p1);

			indices.push_back(p1);
			indices.push_back(p2);
			indices.push_back(p3);
		}
	}

	// Generate indices for top face
	GLuint topVertex = nVertices - nSideVerts;
	for (GLuint i = 0; i < nSides; ++i) {
		indices.push_back(topVertex + i);
		indices.push_back(nVertices - 1);
		indices.push_back(topVertex + i + 1);
	}
}
