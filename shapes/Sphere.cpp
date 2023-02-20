#include "Sphere.h"

Sphere::Sphere(GLfloat nRadius, GLuint nSlices, GLuint nStacks)
{
	setRadius(nRadius);
	setSlices(nSlices);
	setStacks(nStacks);
	bindVertexArrayAndBuffers();
}

// Generate vertices for a sphere
void Sphere::createVertices()
{
	std::vector<GLfloat> verts;
	GLfloat pi = std::acos(-1.0f);
	GLfloat theta = 0.0f;
	GLfloat phi = 0.0f;
	GLfloat thetaStep = 2.0f * pi / (GLfloat)slices;
	GLfloat phiStep = pi / (GLfloat)stacks;

	for (unsigned int i = 0; i <= stacks; i++) {
		phi = (GLfloat)i * phiStep;
		for (unsigned int j = 0; j <= slices; j++) {
			theta = (GLfloat)j * thetaStep;
			GLfloat x = radius * std::sin(phi) * std::cos(theta);
			GLfloat y = radius * std::sin(phi) * std::sin(theta);
			GLfloat z = radius * std::cos(phi);
			verts.push_back(x);
			verts.push_back(y);
			verts.push_back(z);
		}
	}

	setVertices(verts);
}

// Calculate normal coordinates for sphere vertices
void Sphere::createNormals()
{
	std::vector<GLfloat> norms;

	for (unsigned int i = 0; i < vertices.size(); i += 3) {
		GLfloat x = vertices[i];
		GLfloat y = vertices[i + 1];
		GLfloat z = vertices[i + 2];

		GLfloat nx = x / radius;
		GLfloat ny = y / radius;
		GLfloat nz = z / radius;

		GLfloat length = sqrt(nx * nx + ny * ny + nz * nz);
		norms.push_back(nx / length);
		norms.push_back(ny / length);
		norms.push_back(nz / length);
	}

	setNormals(norms);
}

// Calculate texture coordinates for sphere vertices
void Sphere::createTextureCoordinates()
{
	std::vector<GLfloat> texCoords;

	for (unsigned int i = 0; i <= stacks; i++) {
		GLfloat v = 1.0f - (GLfloat)i / (GLfloat)stacks;
		for (unsigned int j = 0; j <= slices; j++) {
			GLfloat u = (GLfloat)j / (GLfloat)slices;
			texCoords.push_back(u);
			texCoords.push_back(v);
		}
	}

	setTextureCoordinates(texCoords);
}

// Calculate indices for each pair of triangles in the vertices
void Sphere::createIndices()
{
	std::vector<GLuint> indices;

	for (unsigned int i = 0; i <= stacks; i++) {
		for (unsigned int j = 0; j <= slices; j++) {
			GLuint p0 = i * (slices + 1) + j;
			GLuint p1 = p0 + slices + 1;

			indices.push_back(p0);
			indices.push_back(p1);
			indices.push_back(p0 + 1);

			indices.push_back(p1);
			indices.push_back(p1 + 1);
			indices.push_back(p0 + 1);
		}
	}

	setIndices(indices);
}