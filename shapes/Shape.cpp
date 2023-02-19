#include "Shape.h"

void Shape::generateInterleavedVertices()
{
	createVertices();
	createNormals();
	createTextureCoordinates();
	createIndices();
	std::vector<float>().swap(interleavedVertices);

	std::size_t i;
	std::size_t j;
	std::size_t count = vertices.size();
	for (i = 0, j = 0; i < count; i += 3, j += 2)
	{
		interleavedVertices.insert(interleavedVertices.end(), &vertices[i], &vertices[i] + 3);
		interleavedVertices.insert(interleavedVertices.end(), &normals[i], &normals[i] + 3);
		interleavedVertices.insert(interleavedVertices.end(), &texCoords[j], &texCoords[j] + 2);
	}
}

void Shape::draw() const
{
	glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, nullptr);
}

void Shape::bindVertexArrayAndBuffers()
{
	const GLint floatsPerVertex = 3;
	const GLint floatsPerNormal = 3;
	const GLint floatsPerTex = 2;

	generateInterleavedVertices();

	glGenVertexArrays(1, &vao); // we can also generate multiple VAOs or buffers at the same time
	glBindVertexArray(vao);

	// Create buffers: first one for the vertex data; second one for the indices
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Activates the buffer
	glBufferData(GL_ARRAY_BUFFER, getInterleavedVertexSize(), &getInterleavedVertices()[0], GL_STATIC_DRAW); // Sends vertex or coordinate data to the GPU

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), &getIndices()[0], GL_STATIC_DRAW);

	// Create Vertex Attribute Pointers for vertex coordinates
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, getStride(), nullptr);
	glEnableVertexAttribArray(0);

	// Create Vertex Attribute Pointers for normals
	glVertexAttribPointer(1, floatsPerNormal, GL_FLOAT, GL_FALSE, getStride(), (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	// Create Vertex Attribute Pointers for texture coordinates
	glVertexAttribPointer(2, floatsPerTex, GL_FLOAT, GL_FALSE, getStride(), (void*)(sizeof(float) * (static_cast<unsigned long long>(floatsPerVertex) + floatsPerNormal)));
	glEnableVertexAttribArray(2);
}