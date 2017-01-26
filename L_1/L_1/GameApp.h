#pragma once
#include "Main.h"
#include "GameObject.h"
#include "Timer.h"

#define COLOR_WHITE 0x00FFFFFF

class CGameApp
{
public:
	CGameApp();
	virtual ~CGameApp();

	//Public fucntions
	LRESULT DisplayWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	bool InitInstance(HANDLE hInstance, LPCTSTR lpCmdLine, int iCmdShow);
	int BeginGame();
	bool ShutDown();

private:
	//Private functions
	bool BuildObjects();
	void FrameAdvance();
	bool CreateDisplay();
	void SetupGameState();
	void AnimateObjects();
	void PresentFrameBuffer();
	void ClearFrameBuffer(ULONG color);
	bool BuildFrameBuffer(ULONG width, ULONG height);
	void DrawPrimitive(CPolygon *pPoly, D3DXMATRIX *pmtxWorld);
	void DrawLine(const D3DXVECTOR3 &vtx1, const D3DXVECTOR3 &vtx2, ULONG color);

	//Private Static Functions
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	//Private Variables
	D3DXMATRIX m_mtxView; //View Matrix
	D3DXMATRIX m_mtxProjection; //Projection Matrix

	CMesh m_Mesh;
	CGameObject m_pObject[2];// The two cubes

	CTimer m_Timer; //Game timer

	HWND m_hWnd; // main windown HWND
	HDC m_hdcFrameBuffer; // frame buffers device context
	HBITMAP m_hbmFrameBuffer; // frame buffer bitmap
	HBITMAP m_hbmSelectOut; // used for selecting out of the DC

	bool m_bRotation1; //Object 1 rotation enabled / disabled
	bool m_bRotation2; //Object 2 rotation enabled / disabled

	ULONG m_nViewX, m_nViewY; //X and Y positions of the render viewport
	ULONG m_nViewWidth, m_nViewHeight; //Height of render viewport
};

