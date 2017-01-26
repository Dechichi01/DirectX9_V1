#pragma once
#include "Vertex.h"

class CPolygon
{
public:
	CPolygon();
	CPolygon(USHORT VertexCount);
	virtual ~CPolygon();

	long AddVertex(USHORT count = 1);

	USHORT m_nVertexCount;
	CVertex *m_pVertex;
};

