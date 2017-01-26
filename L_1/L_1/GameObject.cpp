#include "GameObject.h"

CGameObject::CGameObject()
{
	// Reset / Clear all required values
	m_pMesh = NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
}

CGameObject::CGameObject(CMesh * pMesh)
{
	// Reset / Clear all required values
	D3DXMatrixIdentity(&m_mtxWorld);

	// Set Mesh
	m_pMesh = pMesh;
}

CMesh::CMesh()
{
	// Reset / Clear all required values
	m_nPolygonCount = 0;
	m_pPolygon = NULL;

}

CMesh::CMesh(ULONG Count)
{
	// Reset / Clear all required values
	m_nPolygonCount = 0;
	m_pPolygon = NULL;

	// Add Polygons
	AddPolygon(Count);
}

CMesh::~CMesh()
{
	// Release our mesh components
	if (m_pPolygon)
	{
		// Delete all individual polygons in the array.
		for (ULONG i = 0; i < m_nPolygonCount; i++)
		{
			if (m_pPolygon[i]) delete m_pPolygon[i];

		} // Next Polygon

		  // Free up the array itself
		delete[]m_pPolygon;

	} // End if

	  // Clear variables
	m_pPolygon = NULL;
	m_nPolygonCount = 0;
}

long CMesh::AddPolygon(ULONG Count)
{

	CPolygon ** pPolyBuffer = NULL;

	// Allocate new resized array
	if (!(pPolyBuffer = new CPolygon*[m_nPolygonCount + Count])) return -1;

	// Clear out slack pointers
	ZeroMemory(&pPolyBuffer[m_nPolygonCount], Count * sizeof(CPolygon*));

	// Existing Data?
	if (m_pPolygon)
	{
		// Copy old data into new buffer
		memcpy(pPolyBuffer, m_pPolygon, m_nPolygonCount * sizeof(CPolygon*));

		// Release old buffer
		delete[]m_pPolygon;

	} // End if

	  // Store pointer for new buffer
	m_pPolygon = pPolyBuffer;

	// Allocate new polygon pointers
	for (UINT i = 0; i < Count; i++)
	{
		// Allocate new poly
		if (!(m_pPolygon[m_nPolygonCount] = new CPolygon())) return -1;

		// Increase overall poly count
		m_nPolygonCount++;

	} // Next Polygon

	  // Return first polygon
	return m_nPolygonCount - Count;
}

CPolygon::CPolygon()
{
	// Reset / Clear all required values
	m_nVertexCount = 0;
	m_pVertex = NULL;

}

CPolygon::CPolygon(USHORT Count)
{
	// Reset / Clear all required values
	m_nVertexCount = 0;
	m_pVertex = NULL;

	// Add vertices
	AddVertex(Count);
}

CPolygon::~CPolygon()
{
	// Release our vertices
	if (m_pVertex) delete[]m_pVertex;

	// Clear variables
	m_pVertex = NULL;
	m_nVertexCount = 0;
}

long CPolygon::AddVertex(USHORT Count)
{
	CVertex * pVertexBuffer = NULL;

	// Allocate new resized array
	if (!(pVertexBuffer = new CVertex[m_nVertexCount + Count])) return -1;

	// Existing Data?
	if (m_pVertex)
	{
		// Copy old data into new buffer
		memcpy(pVertexBuffer, m_pVertex, m_nVertexCount * sizeof(CVertex));

		// Release old buffer
		delete[]m_pVertex;

	} // End if

	  // Store pointer for new buffer
	m_pVertex = pVertexBuffer;
	m_nVertexCount += Count;

	// Return first vertex
	return m_nVertexCount - Count;
}