// Pyramid with a square base

#pragma once
#include "Shape.h"
class PyramidSqBase : public Shape
{
public:
	// Constructor and deconstructor
	PyramidSqBase() { bindVertexArrayAndBuffers(); }
	~PyramidSqBase() final = default;

	// Inherited via Shape
	void createVertices() override;
	void createNormals() override;
	void createTextureCoordinates() override;
	void createIndices() override;
};
