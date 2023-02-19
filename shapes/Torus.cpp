#include "Torus.h"
#include <cmath>

Torus::Torus(GLfloat majorRad, GLfloat minorRad, GLuint nSides)
{
	setMajorRadius(majorRad);
	setMinorRadius(minorRad);
	setSides(nSides);
	bindVertexArrayAndBuffers();
}

// Generate vertices for specified torus attributes. Torus lays flat on the x-z plane.
void Torus::createVertices()
{
	std::vector<GLfloat> verts;
	GLfloat pi = std::acos(-1.0f);
	GLfloat theta = 0.0f;
	GLfloat phi = 0.0f;
	GLfloat thetaStep = 2.0f * pi / (GLfloat)sides;
	GLfloat phiStep = 2.0f * pi / (GLfloat)sides;

	for (unsigned int i = 0; i < sides; i++) {
		phi = (GLfloat)i * phiStep;
		for (unsigned int j = 0; j < sides; j++) {
			theta = (GLfloat)j * thetaStep;
			GLfloat x = (majorRadius + minorRadius * std::cos(phi)) * std::cos(theta);
			GLfloat y = minorRadius * std::sin(phi);
			GLfloat z = (majorRadius + minorRadius * std::cos(phi)) * std::sin(theta);
			verts.push_back(x);
			verts.push_back(y);
			verts.push_back(z);
		}
	}

	setVertices(verts);
}

// Calculate normals coordinates for vertices
void Torus::createNormals()
{
	std::vector<GLfloat> normals;
	GLfloat pi = std::acos(-1.0f);
	GLfloat theta = 0.0f;
	GLfloat phi = 0.0f;
	GLfloat thetaStep = 2.0f * pi / (GLfloat)sides;
	GLfloat phiStep = 2.0f * pi / (GLfloat)sides;

	for (unsigned int i = 0; i < sides; i++) {
		phi = (GLfloat)i * phiStep;
		for (unsigned int j = 0; j < sides; j++) {
			theta = (GLfloat)j * thetaStep;
			// Calculate partial derivatives with respect to theta and phi
			GLfloat x_theta = -std::sin(theta);
			GLfloat y_theta = 0.0f;
			GLfloat z_theta = std::cos(theta);

			GLfloat x_phi = std::cos(theta) * (-std::sin(phi) * majorRadius - std::sin(phi + pi / 2.0f) * minorRadius);
			GLfloat y_phi = std::cos(phi) * minorRadius;
			GLfloat z_phi = std::sin(theta) * (-std::sin(phi) * majorRadius - std::sin(phi + pi / 2.0f) * minorRadius);

			// Calculate normal vector as a cross product of the partial derivatives
			GLfloat nx = y_phi * z_theta - z_phi * y_theta;
			GLfloat ny = z_phi * x_theta - x_phi * z_theta;
			GLfloat nz = x_phi * y_theta - y_phi * x_theta;

			// Normalize the vector
			GLfloat len = std::sqrt(nx * nx + ny * ny + nz * nz);
			nx /= len;
			ny /= len;
			nz /= len;

			// Add the normal vector to the list
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);
		}
	}

	setNormals(normals);
}

// Calculate texture coordinates
void Torus::createTextureCoordinates()
{
	std::vector<GLfloat> texCoords;
	GLfloat pi = std::acos(-1.0f);

	for (unsigned int i = 0; i < vertices.size(); i += 3) {
		GLfloat x = vertices[i];
		GLfloat y = vertices[i + 1];
		GLfloat z = vertices[i + 2];

		auto u = static_cast<GLfloat>(atan2(y, x) / (2 * pi) + 0.5);
		auto v = static_cast<GLfloat>(atan2(z, sqrt(x * x + y * y) - majorRadius) / (2 * pi) + 0.5);

		texCoords.push_back(u);
		texCoords.push_back(v);
	}

	setTextureCoordinates(texCoords);
}

// Calculate indices for each pair of triangles in the vertices
void Torus::createIndices()
{
	std::vector<GLuint> indices;

	for (unsigned int i = 0; i < sides; i++) {
		for (unsigned int j = 0; j < sides; j++) {
			GLuint p0 = i * sides + j;
			GLuint p1 = (i + 1) % sides * sides + j;
			GLuint p2 = i * sides + (j + 1) % sides;
			GLuint p3 = (i + 1) % sides * sides + (j + 1) % sides;

			indices.push_back(p0);
			indices.push_back(p2);
			indices.push_back(p1);

			indices.push_back(p1);
			indices.push_back(p2);
			indices.push_back(p3);
		}
	}

	setIndices(indices);
}