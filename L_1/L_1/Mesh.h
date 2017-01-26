#pragma once
#include "Polygon.h"

class CMesh
{
public:
	CMesh();
	CMesh(ULONG count);
	virtual ~CMesh();

	//public functions
	long AddPolygon(ULONG count = 1);

	//public member variables
	ULONG m_nPolygonCount;
	CPolygon **m_pPolygons;
};

