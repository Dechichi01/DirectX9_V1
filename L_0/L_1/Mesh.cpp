#include "stdafx.h"
#include "Mesh.h"


CMesh::CMesh()
{
	m_pPolygons = nullptr;
	m_nPolygonCount = 0;
}

CMesh::CMesh(ULONG count)
{
	m_nPolygonCount = 0;
	m_pPolygons = nullptr;

	AddPolygon(count);
}


CMesh::~CMesh()
{
	if (m_pPolygons)
	{
		for (ULONG i = 0; i < m_nPolygonCount; i++)
		{
			if (m_pPolygons[i]) delete[] m_pPolygons[i];
		}

		delete[] m_pPolygons;
	}
	m_pPolygons = nullptr;
	m_nPolygonCount = 0;
}

long CMesh::AddPolygon(ULONG count)
{
	CPolygon **pPolyBuffer = nullptr;

	if (!(pPolyBuffer = new CPolygon*[m_nPolygonCount + count])) return -1;

	ZeroMemory(&pPolyBuffer[m_nPolygonCount], count * sizeof(CPolygon*));

	if (m_pPolygons)
	{
		memcpy(pPolyBuffer, m_pPolygons, m_nPolygonCount * sizeof(CPolygon*));
		delete[] m_pPolygons;
	}

	m_pPolygons = pPolyBuffer;

	//Allocate new polygons pointers
	for (ULONG i = 0; i < count; i++)
	{
		if (!(m_pPolygons[m_nPolygonCount] = new CPolygon())) return -1;
		m_nPolygonCount++;
	}

	//return firts new polygon index
	return m_nPolygonCount - count;
}
