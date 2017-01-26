#pragma once
#include "Mesh.h"
#include <d3dx9.h>

class CGameObject
{
public:
	CGameObject();
	CGameObject(CMesh *pMesh);
	~CGameObject();

	//public variables
	D3DXMATRIX m_mtxWorld;
	CMesh *m_pMesh;
};

