#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
{
	m_pMesh = nullptr;
	D3DXMatrixIdentity(&m_mtxWorld);
}

CGameObject::CGameObject(CMesh * pMesh)
{
	m_pMesh = pMesh;
	D3DXMatrixIdentity(&m_mtxWorld);
}


CGameObject::~CGameObject()
{
}
