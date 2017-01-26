#include "Vertex.h"


CVertex::CVertex(float fX, float fY, float fZ, ULONG _diffuse)
{
	x = fX;
	y = fY;
	z = fZ;
	diffuse = _diffuse;
}

CVertex::CVertex()
{
	x = y = z = 0;
}



CVertex::~CVertex()
{
}
