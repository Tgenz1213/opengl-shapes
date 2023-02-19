#pragma once
#include "Shape.h"
#include "Cube.h"
#include "PyramidSqBase.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Torus.h"
#include "Plane.h"

// Factory pattern for creating primitive shapes
class ShapeFactory
{
public:
	// Constructor and deconstructor
	ShapeFactory() = default;
	~ShapeFactory() = default;

	// Factory methods
	Cube createCube() const { return Cube(); }
	PyramidSqBase createPyramid() const { return PyramidSqBase(); }
	Sphere createSphere(GLfloat nRadius, GLuint nSlices, GLuint nStacks) const { return Sphere(nRadius, nSlices, nStacks); }
	Torus createTorus(GLfloat majorRad, GLfloat minorRad, GLuint nSides) const { return Torus(majorRad, minorRad, nSides); }
	Cylinder createCylinder(GLfloat baseRadius, GLfloat topRadius, GLfloat height, GLuint nSides, GLuint nStacks) const { return Cylinder(baseRadius, topRadius, height, nSides, nStacks); }
	Plane createPlane() const { return Plane(); }
};
