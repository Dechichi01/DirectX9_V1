#include "GameApp.h"


CGameApp::CGameApp()
{
	m_hWnd = nullptr;
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_hIcon = nullptr;
	m_hMenu = nullptr;
	m_bLostDevice = false;
}


CGameApp::~CGameApp()
{
	ShutDown();
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

bool CGameApp::CreateDisplay()
{
	D3DDISPLAYMODE matchMode;
	CD3DSettingsDlg settingsDlg;
	CMyD3DInit initialize;
	LPTSTR windowTitle = L"Enumeration";
	USHORT width = 400;
	USHORT height = 400;
	RECT rc;

	//First of all create our D3D Object
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3D)//Fatal failure
	{
		MessageBox(m_hWnd, L"No compatible Direct3D object could be created", L"Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return false;
	}

	//Enumerate the system graphics adapter
	if (FAILED(initialize.Enumerate(m_pD3D)))
	{
		MessageBox(m_hWnd, L"Device enumeration failed. The application will now exit.", L"Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return false;
	}

	//Attempt to find good default fullscreen set
	matchMode.Width = 640;
	matchMode.Height = 480;
	matchMode.Format = D3DFMT_UNKNOWN;
	matchMode.RefreshRate = 0;
	initialize.FindBestFullscreenMode(m_D3DSettings, &matchMode);
	//Attempt to find good default windowed set
	initialize.FindBestWindowedMode(m_D3DSettings);

	//Display settings dialog
	int retCode = settingsDlg.ShowDialog(&initialize, &m_D3DSettings);
	if (retCode != IDOK) return false;
	m_D3DSettings = settingsDlg.GetD3DSettings();

	//Create the direct 3d device
	if (FAILED(initialize.CreateDisplay(m_D3DSettings, 0, nullptr, StaticWndProc, windowTitle, width, height, this)))
	{
		MessageBox(m_hWnd, L"Device creation failed. The application will now exit.", L"Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL);
		return false;
	}

	//Retrieve created itens
	m_pD3DDevice = initialize.GetDirect3DDevice();
	m_hWnd = initialize.GetHWND();

	//Load icon and menu
	m_hIcon = LoadIcon((HINSTANCE)GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1));
	m_hMenu = LoadMenu((HINSTANCE)GetModuleHandle(nullptr), MAKEINTRESOURCE(IDR_MENU));

	//Set application icon
	SetClassLong(m_hWnd, GCL_HICON, (long)m_hIcon);

	//Set menu only in windowed mode
	if (m_D3DSettings.windowed) SetMenu(m_hWnd, m_hMenu);

	//Retriev the final client size of the window
	GetClientRect(m_hWnd, &rc);
	m_nViewX = rc.left;
	m_nViewY = rc.top;
	m_nViewWidth = rc.right - rc.left;
	m_nViewHeight = rc.bottom - rc.top;

	ShowWindow(m_hWnd, SW_SHOW);

	return true;
}

void CGameApp::SetupGameState()
{
	D3DXMatrixIdentity(&m_mtxView);

	m_bRotation1 = true;
	m_bRotation2 = true;

	m_bActive = true;
}

void CGameApp::SetupRenderState()
{
	//Store new viewport sizes
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_nViewWidth = rc.right - rc.left;
	m_nViewHeight = rc.bottom - rc.top;

	//Set up new perspetive projection matrix
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

bool CGameApp::ShutDown()
{
	if (m_pD3DDevice) m_pD3DDevice->Release();
	if (m_pD3DDevice) m_pD3D->Release();
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;

	//Destroy menu if attached
	if (m_hMenu) DestroyMenu(m_hMenu);
	m_hMenu = nullptr;
	SetMenu(m_hWnd, nullptr);

	if (m_hWnd) DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	return true;
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

//Handle Windows Messages
LRESULT CGameApp::DisplayWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	CMyD3DInit initialize;

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
				initialize.ResetDisplay(m_pD3DDevice, m_D3DSettings);
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
		case ID_FILE_CHANGEDEVICE:
		{
			CMyD3DInit      initialize;
			CD3DSettingsDlg SettingsDlg;

			// Enumerate the system graphics adapters    
			if (FAILED(initialize.Enumerate(m_pD3D)))
			{
				MessageBox(m_hWnd, L"Device enumeration failed. The application will now exit.", L"Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL);
				PostQuitMessage(0);
				return 0;
			} // End if Failure

			  // Display the settings dialog
			int RetCode = SettingsDlg.ShowDialog(&initialize, &m_D3DSettings);
			if (RetCode != IDOK) break;
			m_D3DSettings = SettingsDlg.GetD3DSettings();

			// Lets Destroy our objects and restart
			if (m_pD3DDevice) m_pD3DDevice->Release();
			m_pD3DDevice = NULL;

			// Create the direct 3d device etc.
			if (FAILED(initialize.CreateDisplay(m_D3DSettings, 0, m_hWnd)))
			{
				MessageBox(m_hWnd, L"Device creation failed. The application will now exit.", L"Fatal Error!", MB_OK | MB_ICONSTOP | MB_APPLMODAL);
				PostQuitMessage(0);
				return 0;
			} // End if failure

			  // Retrieve our new device
			m_pD3DDevice = initialize.GetDirect3DDevice();

			// Set menu only in windowed mode
			if (m_D3DSettings.windowed)
				SetMenu(m_hWnd, m_hMenu);

			// Setup our render states
			SetupRenderState();
			break;
		}
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
		case VK_RETURN:
			if (GetKeyState(VK_SHIFT) & 0xFF00)
			{
				CD3DInitialize initalize;

				//Toggle full screen/windowed
				m_D3DSettings.windowed = !m_D3DSettings.windowed;
				initalize.ResetDisplay(m_pD3DDevice, m_D3DSettings, hWnd);
				SetupRenderState();
				//Set menu onlyin windowed mode (Removed by ResetDisplay automatically in fullscreen)
				if (m_D3DSettings.windowed) SetMenu(m_hWnd, m_hMenu);
			}
			break;
		}
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}

	return 0;
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
			CMyD3DInit initialize;
			initialize.ResetDisplay(m_pD3DDevice, m_D3DSettings, m_hWnd);
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

bool CMyD3DInit::ValidateDisplayMode(const D3DDISPLAYMODE & mode)
{
	//Test display mode
	return !(mode.Width < 640 || mode.Height < 480 || mode.RefreshRate < 60);
}

bool CMyD3DInit::ValidateDevice(const D3DDEVTYPE & type, const D3DCAPS9 & caps)
{
	//Test capabilities (All device types supported)
	return
		(caps.RasterCaps & D3DPRASTERCAPS_DITHER) &&
		(caps.ShadeCaps & D3DPSHADECAPS_COLORGOURAUDRGB) &&
		(caps.PrimitiveMiscCaps & D3DPMISCCAPS_CULLCCW) &&
		(caps.ZCmpCaps & D3DPCMPCAPS_LESSEQUAL);
}

bool CMyD3DInit::ValidateVertexProcessingType(const VERTEXPROCESSING_TYPE & type)
{
	//Application don't need mixed vertex processing
	return !(type == MIXED_VP);
}
