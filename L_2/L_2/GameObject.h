
#pragma once

#include "Main.h"

class CVertex
{
public:
	//
	CVertex(float fX, float fY, float fZ, ULONG ulDiffuse = 0xFF000000)
	{
		x = fX; y = fY; z = fZ; Diffuse = ulDiffuse;
	}

	CVertex()
	{
		x = 0.0f; y = 0.0f; z = 0.0f; Diffuse = 0xFF000000;
	}

	//Public variables
	float       x;          // Vertex Position X Component
	float       y;          // Vertex Position Y Component
	float       z;          // Vertex Position Z Component
	ULONG       Diffuse;    // Diffuse vertex colour component

};


class CPolygon
{
public:

	CPolygon(USHORT VertexCount);
	CPolygon();
	virtual ~CPolygon();

	long        AddVertex(USHORT Count = 1);

	USHORT      m_nVertexCount;         // Number of vertices stored.
	CVertex    *m_pVertex;              // Simple vertex array

};

class CMesh
{
public:

	CMesh(ULONG Count);
	CMesh();
	virtual ~CMesh();

	long        AddPolygon(ULONG Count = 1);

	ULONG       m_nPolygonCount;        // Number of polygons stored
	CPolygon  **m_pPolygon;             // Simply polygon array.

};


class CGameObject
{
public:
	//-------------------------------------------------------------------------
	// Constructors & Destructors for This Class.
	//-------------------------------------------------------------------------
	CGameObject(CMesh * pMesh);
	CGameObject();

	//-------------------------------------------------------------------------
	// Public Variables for This Class
	//-------------------------------------------------------------------------
	D3DXMATRIX  m_mtxWorld;             // Objects world matrix
	CMesh      *m_pMesh;                // Mesh we are instancing

};
