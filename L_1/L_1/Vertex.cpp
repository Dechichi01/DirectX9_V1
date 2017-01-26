#include "Vertex.h"


CVertex::CVertex(float fX, float fY, float fZ)
{
	x = fX;
	y = fY;
	z = fZ;
}

CVertex::CVertex()
{
	x = y = z = 0;
}



CVertex::~CVertex()
{
}
