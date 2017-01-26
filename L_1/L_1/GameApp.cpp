#include "GameApp.h"

#define RANDOM_COLOR COLOR_WHITE | ((rand() * 0xFFFFFF) / RAND_MAX) 


CGameApp::CGameApp()
{
}


CGameApp::~CGameApp()
{
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
				 //Store new viewport sizes
		m_nViewWidth = LOWORD(lParam);
		m_nViewHeight = HIWORD(lParam);

		//Set up new perspective projection matrix
		fAspect = (float)m_nViewWidth / (float)m_nViewHeight;
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(60.f), fAspect, 1.01f, 1000.0f);

		//Rebuild the new frame buffer
		BuildFrameBuffer(m_nViewWidth, m_nViewHeight);
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
	//Create the primary display device
	if (!CreateDisplay()) { ShutDown(); return false; }

	if (!BuildObjects()) { ShutDown(); return false; }

	SetupGameState();

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
	//Destroy frame buffer and associted DC's
	if (m_hdcFrameBuffer && m_hbmFrameBuffer)
	{
		SelectObject(m_hdcFrameBuffer, m_hbmSelectOut);
		DeleteObject(m_hbmFrameBuffer);
		DeleteDC(m_hdcFrameBuffer);
	}
	//Destroy the render window
	if (m_hWnd) DestroyWindow(m_hWnd);

	m_hWnd = nullptr;
	m_hbmFrameBuffer = nullptr;
	m_hdcFrameBuffer = nullptr;

	return true;
}

bool CGameApp::BuildObjects()
{
	//Mesh creation
	CPolygon *pPoly = nullptr;

	if (m_Mesh.AddPolygon(6) < 0) return false;

	//Front face
	pPoly = m_Mesh.m_pPolygons[0];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, 2, -2);
	pPoly->m_pVertex[1] = CVertex(2, 2, -2);
	pPoly->m_pVertex[2] = CVertex(2, -2, -2);
	pPoly->m_pVertex[3] = CVertex(-2, -2, -2);

	//Top face
	pPoly = m_Mesh.m_pPolygons[1];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, 2, -2);
	pPoly->m_pVertex[1] = CVertex(2, 2, -2);
	pPoly->m_pVertex[2] = CVertex(2, 2, 2);
	pPoly->m_pVertex[3] = CVertex(-2, 2, 2);

	//Back face
	pPoly = m_Mesh.m_pPolygons[2];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, -2, 2);
	pPoly->m_pVertex[1] = CVertex(2, -2, 2);
	pPoly->m_pVertex[2] = CVertex(2, 2, 2);
	pPoly->m_pVertex[3] = CVertex(-2, 2, 2);

	//Bottom face
	pPoly = m_Mesh.m_pPolygons[3];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, -2, -2);
	pPoly->m_pVertex[1] = CVertex(2, -2, -2);
	pPoly->m_pVertex[2] = CVertex(2, -2, 2);
	pPoly->m_pVertex[3] = CVertex(-2, -2, 2);

	//Left Face face
	pPoly = m_Mesh.m_pPolygons[4];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(-2, 2, 2);
	pPoly->m_pVertex[1] = CVertex(-2, 2, -2);
	pPoly->m_pVertex[2] = CVertex(-2, -2, -2);
	pPoly->m_pVertex[3] = CVertex(-2, -2, 2);

	//Right face
	pPoly = m_Mesh.m_pPolygons[5];
	if (pPoly->AddVertex(4) < 0) return false;

	pPoly->m_pVertex[0] = CVertex(2, 2, -2);
	pPoly->m_pVertex[1] = CVertex(2, 2, 2);
	pPoly->m_pVertex[2] = CVertex(2, -2, 2);
	pPoly->m_pVertex[3] = CVertex(2, -2, -2);
	//End mesh creation

	//Make the two objects reference this mesh
	m_pObject[0].m_pMesh = &m_Mesh;
	m_pObject[1].m_pMesh = &m_Mesh;

	//Set both objects matrices so that they are offset slightly
	D3DXMatrixTranslation(&m_pObject[0].m_mtxWorld, -3.5f, 2.0f, 14.0f);
	D3DXMatrixTranslation(&m_pObject[1].m_mtxWorld, 3.5f, -2.0f, 14.0f);

	return true;
}

void CGameApp::FrameAdvance()
{
	CMesh *pMesh = nullptr;
	TCHAR lpszFPS[30];

	m_Timer.Tick(60.f);

	AnimateObjects();

	ClearFrameBuffer(COLOR_WHITE);

	//Draw objects
	for (ULONG i = 0; i < 2; i++)
	{
		pMesh = m_pObject[i].m_pMesh;
		//Loop through each polygon
		for (ULONG f = 0; f < pMesh->m_nPolygonCount; f++)
			DrawPrimitive(pMesh->m_pPolygons[f], &m_pObject[i].m_mtxWorld);
	}

	//Show FPS
	m_Timer.GetFrameRate(lpszFPS);
	TextOut(m_hdcFrameBuffer, 5, 5, lpszFPS, strlen((char*)lpszFPS));

	//Everything drawn, present frame buffer
	PresentFrameBuffer();
}

bool CGameApp::CreateDisplay()
{
	LPTSTR windowTitle = _T("Software Render");
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
	wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = windowTitle;
	RegisterClass(&wc);

	//Create window
	m_hWnd = CreateWindow(windowTitle, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, LoadMenu(wc.hInstance, MAKEINTRESOURCE(IDR_MENU)), wc.hInstance, this);

	if (!m_hWnd) return false;//Bail error

							  //Retrieve the final client size of the window
	GetClientRect(m_hWnd, &rect);
	m_nViewX = rect.left;
	m_nViewY = rect.top;
	m_nViewWidth = rect.right - rect.left;
	m_nViewHeight = rect.bottom - rect.top;

	//Build the frame buffer
	if (!BuildFrameBuffer(width, height)) return false;

	ShowWindow(m_hWnd, SW_SHOW);
	return true;
}

void CGameApp::SetupGameState()
{
	float fAspect;
	D3DXMatrixIdentity(&m_mtxView);

	fAspect = (float)m_nViewWidth / (float)m_nViewHeight;
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(60.f), fAspect, 1.01f, 1000.0f);

	m_bRotation1 = true;
	m_bRotation2 = true;
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

void CGameApp::PresentFrameBuffer()
{
	HDC hDC = nullptr;

	hDC = GetDC(m_hWnd);

	//Blit the frame buffer to the screen
	BitBlt(hDC, m_nViewX, m_nViewY, m_nViewWidth, m_nViewHeight, m_hdcFrameBuffer, m_nViewX, m_nViewY, SRCCOPY);

	ReleaseDC(m_hWnd, hDC);
}

void CGameApp::ClearFrameBuffer(ULONG color)
{
	LOGBRUSH logBrush;
	HBRUSH hBrush = nullptr, hOldBrush = nullptr;

	logBrush.lbStyle = BS_SOLID;

	logBrush.lbColor = COLOR_WHITE & RGB2BGR(color);

	hBrush = CreateBrushIndirect(&logBrush);
	if (!hBrush) return;

	hOldBrush = (HBRUSH)SelectObject(m_hdcFrameBuffer, hBrush);

	//Draw rectangle
	Rectangle(m_hdcFrameBuffer, m_nViewX, m_nViewY, m_nViewWidth, m_nViewHeight);
	SelectObject(m_hdcFrameBuffer, hOldBrush);
	DeleteObject(hBrush);

}

bool CGameApp::BuildFrameBuffer(ULONG width, ULONG height)
{
	HDC hDC = GetDC(m_hWnd);
	if (!m_hdcFrameBuffer) m_hdcFrameBuffer = CreateCompatibleDC(hDC);

	m_hbmFrameBuffer = CreateCompatibleBitmap(hDC, width, height);
	if (!m_hbmFrameBuffer) return false;

	//Select new bitmap handler and save previous for later restore
	m_hbmSelectOut = (HBITMAP)SelectObject(m_hdcFrameBuffer, m_hbmFrameBuffer);

	ReleaseDC(m_hWnd, hDC);
	SetBkMode(m_hdcFrameBuffer, TRANSPARENT);//Set the frame buffer so it renders correctly (not shown)

	return true;
}

//Responsible for transforming polygons from modelSpace to screen space and draw them into the screen
void CGameApp::DrawPrimitive(CPolygon * pPoly, D3DXMATRIX * pmtxWorld)
{
	D3DXVECTOR3 vtxPrevious, vtxCurrent;

	//Loop round each vertex transforming as we go
	for (USHORT v = 0; v < pPoly->m_nVertexCount + 1; v++)
	{
		vtxCurrent = (D3DXVECTOR3&)pPoly->m_pVertex[v % pPoly->m_nVertexCount];
		//Transform vertex from Model Space to World Space
		D3DXVec3TransformCoord(&vtxCurrent, &vtxCurrent, pmtxWorld);
		//Transform vertex from World Space to View Space
		D3DXVec3TransformCoord(&vtxCurrent, &vtxCurrent, &m_mtxView);
		D3DXVec3TransformCoord(&vtxCurrent, &vtxCurrent, &m_mtxProjection);//Apply FOV
																		   //Transform vertex from View Space to Screen Space
		vtxCurrent.x = vtxCurrent.x *m_nViewWidth / 2 + m_nViewX + m_nViewWidth / 2;
		vtxCurrent.y = -vtxCurrent.y  *m_nViewHeight / 2 + m_nViewY + m_nViewHeight / 2;

		//Draw lines (always from previous vertex to current vertex)
		if (v > 0)
			DrawLine(vtxPrevious, vtxCurrent, 0);

		vtxPrevious = vtxCurrent;
	}
}

void CGameApp::DrawLine(const D3DXVECTOR3 & vtx1, const D3DXVECTOR3 & vtx2, ULONG color)
{
	LOGPEN logPen;
	HPEN hPen = nullptr, hOldPen = nullptr;

	//set up the rendering pen
	logPen.lopnStyle = PS_SOLID;
	logPen.lopnWidth.x = logPen.lopnWidth.y = 1;
	logPen.lopnColor = COLOR_WHITE & RGB2BGR(color);

	//Create the rendering pen
	hPen = CreatePenIndirect(&logPen);
	if (!hPen) return;

	//Select into the frame buffer DC
	hOldPen = (HPEN)SelectObject(m_hdcFrameBuffer, hPen);

	//Draw line
	MoveToEx(m_hdcFrameBuffer, (long)vtx1.x, (long)vtx1.y, NULL);
	LineTo(m_hdcFrameBuffer, (long)vtx2.x, (long)vtx2.y);

	//Destroy rendering pen
	SelectObject(m_hdcFrameBuffer, hOldPen);
	DeleteObject(hPen);
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
