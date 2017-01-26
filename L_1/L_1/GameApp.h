#pragma once
#include "Main.h"
#include "GameObject.h"
#include "Timer.h"

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
	D3DFORMAT FindDepthStencilFormat(ULONG adapterOrdinal, D3DDISPLAYMODE displayMode, D3DDEVTYPE devType);
	void FrameAdvance();
	void ProcessInput();
	bool InitDirect3D();
	bool CreateDisplay();
	void SetupGameState();
	void SetupRenderState();
	void AnimateObjects();

	//Private Static Functions
	static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	//DirectX Variables
	LPDIRECT3D9 m_pD3D;//Direct3D object
	LPDIRECT3DDEVICE9 m_pD3DDevice;//Device object
	D3DPRESENT_PARAMETERS m_D3DPresentParams;

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

	bool m_bActive;
	bool m_bLostDevice;
	bool m_bRotation1; //Object 1 rotation enabled / disabled
	bool m_bRotation2; //Object 2 rotation enabled / disabled

	ULONG m_nViewX, m_nViewY; //X and Y positions of the render viewport
	ULONG m_nViewWidth, m_nViewHeight; //Height of render viewport
};

