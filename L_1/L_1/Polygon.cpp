#include "stdafx.h"
#include "Polygon.h"


CPolygon::CPolygon()
{
	m_nVertexCount = 0;
	m_pVertex = nullptr;
}

CPolygon::CPolygon(USHORT VertexCount)
{
	m_nVertexCount = 0;
	m_pVertex = nullptr;

	AddVertex(VertexCount);
}

CPolygon::~CPolygon()
{
	if (m_pVertex) delete[] m_pVertex;

	m_nVertexCount = 0;
	m_pVertex = nullptr;
}

long CPolygon::AddVertex(USHORT count)
{
	CVertex* pVertexBuffer = nullptr;
	if (!(pVertexBuffer = new CVertex[m_nVertexCount + count])) return -1;

	if (m_pVertex)//Existing data?
	{
		//Copy existing data to pVertexBuffer
		memcpy(pVertexBuffer, m_pVertex, m_nVertexCount * sizeof(CVertex));
		//Delete old buffer
		delete[] m_pVertex;
	}

	m_pVertex = pVertexBuffer;
	m_nVertexCount += count;

	//Return first new vertex index
	return m_nVertexCount - count;
}
