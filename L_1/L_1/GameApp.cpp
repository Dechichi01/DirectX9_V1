#include "GameApp.h"


CGameApp::CGameApp()
{
	m_hWnd = nullptr;
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_bLostDevice = false;
}


CGameApp::~CGameApp()
{
	ShutDown();
}

//Handle Windows Messages
LRESULT CGameApp::DisplayWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	float fAspect;

	switch (Message)
	{
	case WM_CREATE:
		break;
	case WM_CLOSE:
		PostQuitMessage(0);//Will send a WM_QUIT message to the window (is retrieve by CGameApp::BeginGame to break from the infinite loop)
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE://User resized window
		if (wParam == SIZE_MINIMIZED) m_bActive = false;
		else
		{
			m_bActive = true;
			//Store new viewport sizes
			m_nViewWidth = LOWORD(lParam);
			m_nViewHeight = HIWORD(lParam);
			if (m_pD3DDevice)
			{
				m_pD3DDevice->Reset(&m_D3DPresentParams);
				SetupRenderState();
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_ANIM_ROTATION1://Toggle rotation 1
			m_bRotation1 = !m_bRotation1;
			CheckMenuItem(GetMenu(m_hWnd), ID_ANIM_ROTATION1, MF_BYCOMMAND | (m_bRotation1) ? MF_CHECKED : MF_UNCHECKED);
			break;
		case ID_ANIM_ROTATION2://Toggle rotation 2
			m_bRotation2 = !m_bRotation2;
			CheckMenuItem(GetMenu(m_hWnd), ID_ANIM_ROTATION2, MF_BYCOMMAND | (m_bRotation2) ? MF_CHECKED : MF_UNCHECKED);
			break;
		case ID_EXIT:
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
			return 0;
		}
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}

	return 0;
}

bool CGameApp::InitInstance(HANDLE hInstance, LPCTSTR lpCmdLine, int iCmdShow)
{
	//Create the primary display device (also Init DirectX)
	if (!CreateDisplay()) { ShutDown(); return false; }

	if (!BuildObjects()) { ShutDown(); return false; }

	SetupGameState();

	SetupRenderState();

	return true;
}

int CGameApp::BeginGame()
{
	MSG msg;

	while (true)//Until quit message is received
	{
		//Did we receive a message?
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			FrameAdvance();
	}

	return 0;
}

bool CGameApp::ShutDown()
{
	if (m_pD3DDevice) m_pD3DDevice->Release();
	if (m_pD3DDevice) m_pD3D->Release();
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;

	if (m_hWnd) DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	return true;
}

bool CGameApp::BuildObjects()
{
	CPolygon * pPoly = NULL;

	// Seed the random number generator
	srand(timeGetTime());

	// Add 6 polygons to this mesh.
	if (m_Mesh.AddPolygon(6) < 0) return false;

	// Front Face
	pPoly = m_Mesh.m_pPolygon[0];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, 2, -2, RANDOM_COLOR);
	pPoly->m_pVertex[1] = CVertex(2, 2, -2, RANDOM_COLOR);
	pPoly->m_pVertex[2] = CVertex(2, -2, -2, RANDOM_COLOR);
	pPoly->m_pVertex[3] = CVertex(-2, -2, -2, RANDOM_COLOR);

	// Top Face
	pPoly = m_Mesh.m_pPolygon[1];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, 2, 2, RANDOM_COLOR);
	pPoly->m_pVertex[1] = CVertex(2, 2, 2, RANDOM_COLOR);
	pPoly->m_pVertex[2] = CVertex(2, 2, -2, RANDOM_COLOR);
	pPoly->m_pVertex[3] = CVertex(-2, 2, -2, RANDOM_COLOR);

	// Back Face
	pPoly = m_Mesh.m_pPolygon[2];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, -2, 2, RANDOM_COLOR);
	pPoly->m_pVertex[1] = CVertex(2, -2, 2, RANDOM_COLOR);
	pPoly->m_pVertex[2] = CVertex(2, 2, 2, RANDOM_COLOR);
	pPoly->m_pVertex[3] = CVertex(-2, 2, 2, RANDOM_COLOR);

	// Bottom Face
	pPoly = m_Mesh.m_pPolygon[3];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, -2, -2, RANDOM_COLOR);
	pPoly->m_pVertex[1] = CVertex(2, -2, -2, RANDOM_COLOR);
	pPoly->m_pVertex[2] = CVertex(2, -2, 2, RANDOM_COLOR);
	pPoly->m_pVertex[3] = CVertex(-2, -2, 2, RANDOM_COLOR);

	// Left Face
	pPoly = m_Mesh.m_pPolygon[4];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, 2, 2, RANDOM_COLOR);
	pPoly->m_pVertex[1] = CVertex(-2, 2, -2, RANDOM_COLOR);
	pPoly->m_pVertex[2] = CVertex(-2, -2, -2, RANDOM_COLOR);
	pPoly->m_pVertex[3] = CVertex(-2, -2, 2, RANDOM_COLOR);

	// Right Face
	pPoly = m_Mesh.m_pPolygon[5];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(2, 2, -2, RANDOM_COLOR);
	pPoly->m_pVertex[1] = CVertex(2, 2, 2, RANDOM_COLOR);
	pPoly->m_pVertex[2] = CVertex(2, -2, 2, RANDOM_COLOR);
	pPoly->m_pVertex[3] = CVertex(2, -2, -2, RANDOM_COLOR);

	// Our two objects should reference this mesh
	m_pObject[0].m_pMesh = &m_Mesh;
	m_pObject[1].m_pMesh = &m_Mesh;

	// Set both objects matrices so that they are offset slightly
	D3DXMatrixTranslation(&m_pObject[0].m_mtxWorld, -3.5f, 2.0f, 14.0f);
	D3DXMatrixTranslation(&m_pObject[1].m_mtxWorld, 3.5f, -2.0f, 14.0f);

	// Success!
	return true;
}

D3DFORMAT CGameApp::FindDepthStencilFormat(ULONG adapterOrdinal, D3DDISPLAYMODE displayMode, D3DDEVTYPE devType)
{
	// Test for 32 bith depth buffer
	if (SUCCEEDED(m_pD3D->CheckDeviceFormat(adapterOrdinal, devType, displayMode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D32)))
	{
		if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(adapterOrdinal, devType, displayMode.Format, displayMode.Format, D3DFMT_D32))) return D3DFMT_D32;
	} // End if 32bpp Available

	// Test for 24 bit depth buffer
	if (SUCCEEDED(m_pD3D->CheckDeviceFormat(adapterOrdinal, devType, displayMode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D24X8)))
	{
		if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(adapterOrdinal, devType, displayMode.Format, displayMode.Format, D3DFMT_D24X8))) return D3DFMT_D24X8;
	} // End if 24bpp Available

	  // Test for 16 bit depth buffer
	if (SUCCEEDED(m_pD3D->CheckDeviceFormat(adapterOrdinal, devType, displayMode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, D3DFMT_D16)))
	{
		if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(adapterOrdinal, devType, displayMode.Format, displayMode.Format, D3DFMT_D16))) return D3DFMT_D16;

	} // End if 16bpp Available

	//No depth buffer supported
	return D3DFMT_UNKNOWN;
}

void CGameApp::FrameAdvance()
{
	CMesh *pMesh = nullptr;
	TCHAR lpszFPS[30];

	m_Timer.Tick(60.f);

	if (!m_bActive) return;

	if (m_bLostDevice)
	{
		//Can we reset the device yet?
		HRESULT hRet = m_pD3DDevice->TestCooperativeLevel();
		if (hRet == D3DERR_DEVICENOTRESET)//Device available, but not reset. Restore device
		{
			m_pD3DDevice->Reset(&m_D3DPresentParams);
			SetupRenderState();
			m_bLostDevice = false;
		}
		else return;//Device not available yet
	}	

	ProcessInput();

	AnimateObjects();

	//Clear frame buffer before render scene
	m_pD3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, COLOR_WHITE, 1.0f, 0);

	//Start scene rendering
	m_pD3DDevice->BeginScene();

	//Loop trhogut each object
	for (ULONG i = 0; i < 2; i++)
	{
		pMesh = m_pObject[i].m_pMesh;

		m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_pObject[i].m_mtxWorld);

		//Loop through each polygon
		for (ULONG f = 0; f < pMesh->m_nPolygonCount; f++)
		{
			CPolygon* pPolygon = pMesh->m_pPolygon[f];

			m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, pPolygon->m_nVertexCount - 2, pPolygon->m_pVertex, sizeof(CVertex));
		}//Next polygon
	}//Next Object

	m_pD3DDevice->EndScene();
	
	//Present the buffer
	if (FAILED(m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr)))//wasn't able to present scene, application was minimized
		m_bLostDevice = true;
}

void CGameApp::ProcessInput()
{
	if (GetKeyState(VK_LEFT) & 0xFF00)
		m_mtxView._41 += 25.f *m_Timer.GetTimeElapsed();
	if (GetKeyState(VK_RIGHT) & 0xFF00)
		m_mtxView._41 -= 25.f *m_Timer.GetTimeElapsed();

	if (m_pD3DDevice) m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

bool CGameApp::InitDirect3D()
{
	D3DPRESENT_PARAMETERS presentParams;
	D3DCAPS9 caps;
	D3DDISPLAYMODE currentMode;
	HRESULT hRet;

	//First of all create our D3D Object
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3D) return false;

	//Fill out a simple set of present parameters
	ZeroMemory(&presentParams, sizeof(D3DPRESENT_PARAMETERS));

	//Query default adapter display mod
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &currentMode);
	presentParams.BackBufferFormat = currentMode.Format;//select back buffer format
	presentParams.EnableAutoDepthStencil = true;//We want to create a depth buffer attached to the frame buffer at device creation
	presentParams.AutoDepthStencilFormat = FindDepthStencilFormat(D3DADAPTER_DEFAULT, currentMode, D3DDEVTYPE_HAL);//Find the correct Depth Stencl format

	presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParams.Windowed = true;

	//Query hardware capabilities
	unsigned long ulFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//Check if hardware T&L is available
	ZeroMemory(&caps, sizeof(D3DCAPS9));
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		ulFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	//Attempt to creat a HAL device
	if (FAILED(hRet = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, ulFlags, &presentParams, &m_pD3DDevice)))
	{
		//Failed, create a HEL
		presentParams.AutoDepthStencilFormat = FindDepthStencilFormat(D3DADAPTER_DEFAULT, currentMode, D3DDEVTYPE_REF);
		// Check if hardware T&L is available
		ZeroMemory(&caps, sizeof(D3DCAPS9));
		ulFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &caps);

		//Eventhough it's just a HEL, software vertex varies if it's emulating T&L and rasterization or just rasterization
		if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			ulFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

		//Attempt to create a REF device
		if (FAILED(hRet - m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd, ulFlags, &presentParams, &m_pD3DDevice)))
			return false;//Failed
	}

	m_D3DPresentParams = presentParams;

	return true;
}

bool CGameApp::CreateDisplay()
{
	LPTSTR windowTitle = _T("Initialization");
	USHORT width = 400;
	USHORT height = 400;
	HDC hDC = nullptr;
	RECT rect;

	//Register the new windows window class.
	WNDCLASS wc;
	wc.style = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = StaticWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = (HINSTANCE)GetModuleHandle(nullptr);
	wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = windowTitle;
	RegisterClass(&wc);

	//Create window
	m_hWnd = CreateWindow(windowTitle, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		width, height, nullptr, LoadMenu(wc.hInstance, MAKEINTRESOURCE(IDR_MENU)), wc.hInstance, this);

	if (!m_hWnd) return false;//Bail error

	//Retrieve the final client size of the window
	GetClientRect(m_hWnd, &rect);
	m_nViewX = rect.left;
	m_nViewY = rect.top;
	m_nViewWidth = rect.right - rect.left;
	m_nViewHeight = rect.bottom - rect.top;

	ShowWindow(m_hWnd, SW_SHOW);

	if (!InitDirect3D()) return false;

	return true;
}

void CGameApp::SetupGameState()
{
	m_bRotation1 = true;
	m_bRotation2 = true;

	m_bActive = true;
}

void CGameApp::SetupRenderState()
{
	//Setup view and projection matrices
	D3DXMatrixIdentity(&m_mtxView);
	float fAspect = (float)m_nViewWidth / (float)m_nViewHeight;
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(60.f), fAspect, 1.f, 1000.f);//near clip plane and far cip plane -> Camera frustum

	//Setup our D3D Device initial state
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);//Z depth buffer
	m_pD3DDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Setup our vertes FVF flags
	m_pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	//Setup our matrices
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}

void CGameApp::AnimateObjects()
{
	D3DXMATRIX mtxYaw, mtxPitch, mtxRoll, mtxRotate;//y,x,z
	float rotationYaw, rotationPitch, rotationRoll;

	if (m_bRotation1)
	{
		rotationYaw = D3DXToRadian(75.f*m_Timer.GetTimeElapsed());
		rotationPitch = D3DXToRadian(50.f*m_Timer.GetTimeElapsed());
		rotationRoll = D3DXToRadian(25.f*m_Timer.GetTimeElapsed());

		//Multiplication order: currentMtx * rotY * rotX * rotZ
		D3DXMatrixRotationYawPitchRoll(&mtxRotate, rotationYaw, rotationPitch, rotationRoll);
		//Multiplcation order: rotationMtx * worldMtx
		m_pObject[0].m_mtxWorld = mtxRotate * m_pObject[0].m_mtxWorld;
	}
	if (m_bRotation2)
	{
		rotationYaw = D3DXToRadian(-25.f*m_Timer.GetTimeElapsed());
		rotationPitch = D3DXToRadian(50.f*m_Timer.GetTimeElapsed());
		rotationRoll = D3DXToRadian(-75.f*m_Timer.GetTimeElapsed());

		//Multiplication order: currentMtx * rotY * rotX * rotZ
		D3DXMatrixRotationYawPitchRoll(&mtxRotate, rotationYaw, rotationPitch, rotationRoll);
		//Multiplcation order: rotationMtx * worldMtx
		m_pObject[1].m_mtxWorld = mtxRotate * m_pObject[1].m_mtxWorld;
	}
}

LRESULT CGameApp::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	//Every windows has a 4 byte user data that can be set using SetWindowLong. We set it to a pointer to the CGameApp instance
	//that created this window
	if (Message == WM_CREATE)
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR*) lParam)->lpCreateParams);

	CGameApp *gameApp = (CGameApp*)GetWindowLong(hWnd, GWL_USERDATA);
	if (gameApp)
		return gameApp->DisplayWndProc(hWnd, Message, wParam, lParam);//Pass the message handling to the GameApp handle function

	return DefWindowProc(hWnd, Message, wParam, lParam);

}
