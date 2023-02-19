#pragma once

#include "framework.h"

// Abstract class for creating user-defined primitive shapes
class Shape
{
public:
	// Constructor and deconstructor
	Shape() = default;
	virtual ~Shape() = default;

	// Create vertex and index data for shape, must be implemented in derived classes
	virtual void createVertices() = 0;
	virtual void createNormals() = 0;
	virtual void createTextureCoordinates() = 0;
	virtual void createIndices() = 0;

	// Methods
	void bindVertexArrayAndBuffers();
	void generateInterleavedVertices();

	// Setters
	void setVertices(std::vector<GLfloat> const& vert) { vertices = vert; }
	void setNormals(std::vector<GLfloat> const& norm) { normals = norm; }
	void setTextureCoordinates(std::vector<GLfloat> const& tex) { texCoords = tex; }
	void setIndices(std::vector<GLuint> const& index) { indices = index; }

	// Getters
	glm::mat4 getModel() const { return model; }
	GLint getStride() const { return stride; }
	GLuint getInterleavedVertexSize() const { return (GLuint)interleavedVertices.size() * sizeof(unsigned int); }
	GLuint getInterleavedVertexCount() const { return sizeof(interleavedVertices) / (sizeof(interleavedVertices[0]) * 8); }
	GLuint getIndexCount() const { return (GLuint)indices.size(); }
	GLuint getIndexSize() const { return (GLuint)indices.size() * sizeof(unsigned int); }

	std::vector <GLfloat> getInterleavedVertices() const { return interleavedVertices; }
	std::vector<GLuint> getIndices() const { return indices; }

	// Draw
	void draw() const;

	// Attributes
	GLuint vao{};
	GLuint vbo{};
	GLuint ebo{};
	GLuint texId{};
	GLint stride = 32;
	glm::mat4 model = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));			// Use by View to transform
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> texCoords;
	std::vector<GLfloat> interleavedVertices;	// vertices-normals-texCords
	std::vector<GLuint> indices;
};
