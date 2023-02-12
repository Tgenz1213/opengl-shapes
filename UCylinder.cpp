#include "UCylinder.h"

void UCylinder::createVertices()
{
	const float* vertexData = cylinder.getVertices();
	int dataSize = cylinder.getVertexCount() * 3;
	for (int i = 0; i < dataSize; i++)
	{
		this->vertices.push_back(vertexData[i]);
	}
}

void UCylinder::createNormals()
{
	const float* vertexData = cylinder.getNormals();
	int dataSize = cylinder.getNormalCount() * 3;
	for (int i = 0; i < dataSize; i++)
	{
		this->normals.push_back(vertexData[i]);
	}
}

void UCylinder::createTextureCoordinates()
{
	const float* vertexData = cylinder.getTexCoords();
	int dataSize = cylinder.getTexCoordCount() * 2;
	for (int i = 0; i < dataSize; i++)
	{
		this->texCoords.push_back(vertexData[i]);
	}
}

void UCylinder::createIndices()
{
	indices.assign(cylinder.getIndices(), cylinder.getIndices() + cylinder.getIndexSize());
}