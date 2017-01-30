#pragma once
#include "stdafx.h"
class CVertex
{
public:
	CVertex(float fX, float fY, float fZ, ULONG _diffuse = 0xFF000000);
	CVertex();

	~CVertex();

	//Public variables
	float x, y, z;
	ULONG diffuse;//Diffuse vertex color component
};

