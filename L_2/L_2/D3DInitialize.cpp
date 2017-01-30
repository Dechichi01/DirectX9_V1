#include "D3DInitialize.h"

//Global variables
const ULONG validAdapterFormatCount = 3;
const D3DFORMAT validAdapterFormats[3] = { D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5 };//At least one of the adapter formats will be supported

const ULONG deviceTypeCount = 3;
const D3DDEVTYPE deviceTypes[3] = { D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_REF };

const ULONG backBufferFormatCount = 11;
const D3DFORMAT backBufferFormats[11] = {   D3DFMT_R8G8B8, D3DFMT_A8R8G8B8,
											D3DFMT_X8R8G8B8, D3DFMT_R5G6B5,
											D3DFMT_A1R5G5B5, D3DFMT_X1R5G5B5,
											D3DFMT_R3G3B2, D3DFMT_A8R3G3B2,
											D3DFMT_X4R4G4B4, D3DFMT_A4R4G4B4,
											D3DFMT_A2B10G10R10 };

const ULONG depthStencilFormatCount = 6;
const D3DFORMAT depthStencilFormats[6] = { D3DFMT_D16, D3DFMT_D15S1, D3DFMT_D24X8, D3DFMT_D24S8, D3DFMT_D24X4S4, D3DFMT_D32 };

const ULONG multiSampleTypeCount = 17;
const D3DMULTISAMPLE_TYPE multiSampleTypes[17] ={ 
												D3DMULTISAMPLE_NONE, D3DMULTISAMPLE_NONMASKABLE, 
												D3DMULTISAMPLE_2_SAMPLES , D3DMULTISAMPLE_3_SAMPLES,       
												D3DMULTISAMPLE_4_SAMPLES , D3DMULTISAMPLE_5_SAMPLES,  
												D3DMULTISAMPLE_6_SAMPLES , D3DMULTISAMPLE_7_SAMPLES,   
												D3DMULTISAMPLE_8_SAMPLES , D3DMULTISAMPLE_9_SAMPLES,     
												D3DMULTISAMPLE_10_SAMPLES, D3DMULTISAMPLE_11_SAMPLES,      
												D3DMULTISAMPLE_12_SAMPLES, D3DMULTISAMPLE_13_SAMPLES,       
												D3DMULTISAMPLE_14_SAMPLES, D3DMULTISAMPLE_15_SAMPLES,                               
												D3DMULTISAMPLE_16_SAMPLES };

const ULONG presentIntervalCount = 6;
const ULONG presentIntervals[6] = {
									D3DPRESENT_INTERVAL_IMMEDIATE,    
									D3DPRESENT_INTERVAL_DEFAULT,        
									D3DPRESENT_INTERVAL_ONE,        
									D3DPRESENT_INTERVAL_TWO,    
									D3DPRESENT_INTERVAL_THREE,   
									D3DPRESENT_INTERVAL_FOUR };

CD3DEnumAdapter::~CD3DEnumAdapter()
{
	//Release all devices
	for (ULONG i = 0; i < devices.size(); i++)
		if (devices[i]) delete devices[i];

	modes.clear();
	devices.clear();
}

CD3DEnumDevice::~CD3DEnumDevice()
{
	for (ULONG i = 0; i < options.size(); i++)
		if (options[i]) delete options[i];

	// Clear Vectors
	options.clear();
}

CD3DEnumDeviceOptions::~CD3DEnumDeviceOptions()
{
	depthFormats.clear();
	multiSampleTypes.clear();
	presentIntervals.clear();
	vertexProcessingTypes.clear();
}

CD3DInitialize::CD3DInitialize()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
}


CD3DInitialize::~CD3DInitialize()
{
	if (m_pD3D) m_pD3D->Release();
	m_pD3D = nullptr;

	if (m_pD3DDevice) m_pD3DDevice->Release();
	m_pD3DDevice = nullptr;

	for (ULONG i = 0; i < m_vpAdapters.size(); i++)
		if (m_vpAdapters[i]) delete m_vpAdapters[i];

	m_vpAdapters.clear();
}

HRESULT CD3DInitialize::Enumerate(LPDIRECT3D9 pD3D)
{
	HRESULT hRet;

	//Store de D3D object
	m_pD3D = pD3D;
	if (!m_pD3D) return E_FAIL;

	//We have made a copy of pointer to increase reference count
	m_pD3D->AddRef();

	//Enumerate the adapters
	if (FAILED(hRet = EnumerateAdapters())) return hRet;

	return S_OK;
}

HRESULT CD3DInitialize::CreateDisplay(CD3DSettings & D3DSettings, ULONG flags, HWND hWnd, WNDPROC pWndProc, LPCTSTR title, ULONG width, ULONG height, LPVOID lParam)
{
	ULONG createFlags = 0;
	CD3DSettings::Settings* pSettings = D3DSettings.GetSettings();

	if (!hWnd)//No window handle passed, create a new window
	{
		//Register the new windows window class
		WNDCLASS wc;
		wc.style = CS_BYTEALIGNCLIENT | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = pWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = (HINSTANCE)GetModuleHandle(nullptr);
		wc.hIcon = nullptr;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = title;
		RegisterClass(&wc);

		ULONG left = CW_USEDEFAULT, top = CW_USEDEFAULT;
		ULONG style = WS_OVERLAPPEDWINDOW;

		//Create the rendering window
		if (!D3DSettings.windowed)//Fullscreen
		{
			left = top = 0;
			width = pSettings->displayMode.Width;
			height = pSettings->displayMode.Height;
			style = WS_VISIBLE | WS_POPUP;
		}
		//Create the window
		m_hWnd = CreateWindow(title, title, style, left, top, width, height, nullptr, nullptr, wc.hInstance, lParam);
		if (!m_hWnd) return E_FAIL;
	}
	else
	{
		m_hWnd = hWnd;
		if (!D3DSettings.windowed)//Full screen
		{
			SetMenu(m_hWnd, nullptr);//Remove menu
			SetWindowLong(m_hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			SetWindowPos(m_hWnd, nullptr, 0, 0, pSettings->displayMode.Width, pSettings->displayMode.Height, SWP_NOZORDER);
		}
		else//Windowed
		{
			RECT rc;
			//Get windows client rectangle
			GetWindowRect(hWnd, &rc);
			//Setup the window properties
			SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_NOTOPMOST, rc.left, rc.top, (rc.right - rc.left), (rc.bottom - rc.top), SWP_NOACTIVATE | SWP_SHOWWINDOW);
		}
	}

	//Build our present parameters
	D3DPRESENT_PARAMETERS d3dpp = BuildPresentParameters(D3DSettings);

	//Build our creatino flags
	if (pSettings->vertexProcessingType == PURE_HARDWARE_VP)
		createFlags |= D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else if (pSettings->vertexProcessingType == HARDWARE_VP)
		createFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else if (pSettings->vertexProcessingType == MIXED_VP)
		createFlags |= D3DCREATE_MIXED_VERTEXPROCESSING;
	else if (pSettings->vertexProcessingType == SOFTWARE_VP)
		createFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//Create the device
	m_pD3DDevice = nullptr;
	HRESULT hRet = m_pD3D->CreateDevice(pSettings->adapterOrdinal, pSettings->deviceType, m_hWnd, createFlags, &d3dpp, &m_pD3DDevice);
	//Did the creation fail?
	if (FAILED(hRet))
	{
		if (m_pD3DDevice) m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
		return hRet;
	}

	//Sucess
	return S_OK;
}

HRESULT CD3DInitialize::ResetDisplay(LPDIRECT3DDEVICE9 pD3DDevice, CD3DSettings& D3DSettings, HWND hWnd)
{
	D3DPRESENT_PARAMETERS d3dpp = BuildPresentParameters(D3DSettings);
	CD3DSettings::Settings* pSettings = D3DSettings.GetSettings();

	if (hWnd)
	{
		if (!D3DSettings.windowed)//Full screen
		{
			SetMenu(hWnd, nullptr);
			SetWindowLong(hWnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);
			SetWindowPos(hWnd, nullptr, 0, 0, pSettings->displayMode.Width, pSettings->displayMode.Height, SWP_NOZORDER);
		}
		else //Windowed
		{
			SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hWnd, HWND_NOTOPMOST, 50, 50, 400, 400, SWP_NOACTIVATE | SWP_SHOWWINDOW);
		}
	}

	HRESULT hRet = pD3DDevice->Reset(&d3dpp);
	
	return (FAILED(hRet)) ? hRet : S_OK;
}

bool CD3DInitialize::FindBestWindowedMode(CD3DSettings & D3DSettings, bool bRequireHAL, bool bRequireREF)
{
	ULONG i, j, k;
	D3DDISPLAYMODE displayMode;
	CD3DEnumAdapter* pBestAdapter = nullptr;
	CD3DEnumDevice* pBestDevice = nullptr;
	CD3DEnumDeviceOptions* pBestOptions = nullptr;
	CD3DSettings::Settings* pSettings = nullptr;

	//Retrieve the primary adapters display mode
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
	//Loop through each adapter
	for (i = 0; i < GetAdapterCount(); i++)
	{
		CD3DEnumAdapter* pAdapter = m_vpAdapters[i];
		//Loop through each device
		for (j = 0; j < pAdapter->devices.size(); j++)
		{
			CD3DEnumDevice* pDevice = pAdapter->devices[j];
			//Skip if ths is not the required type
			if ((bRequireHAL && pDevice->DeviceType != D3DDEVTYPE_HAL) ||
				(bRequireREF && pDevice->DeviceType != D3DDEVTYPE_REF)) continue;

			//Loop through each option set
			for (k = 0; k < pDevice->options.size(); k++)
			{
				CD3DEnumDeviceOptions* pOptions = pDevice->options[k];
				//Determine if back buffer format matches adapter
				bool matchedBB = pOptions->backBufferFormat == pOptions->adapterFormat;
				//Skip if it's full screen or types don't match
				if (!pOptions->windowed || pOptions->adapterFormat != displayMode.Format) continue;

				//Check if we want to store this as the best mode
				if (pBestOptions == nullptr ||//we haven't added a best mode yet
					(pOptions->deviceType == D3DDEVTYPE_HAL && matchedBB) ||//HAL that matches BB and adapter format
					(pBestOptions->deviceType != D3DDEVTYPE_HAL && pOptions->deviceType == D3DDEVTYPE_HAL))
				{
					//Store best so far
					pBestAdapter = pAdapter;
					pBestDevice = pDevice;
					pBestOptions = pOptions;
					if (pOptions->deviceType == D3DDEVTYPE_HAL && matchedBB)//This window looks great, no need to continue
						goto EndWindowedDeviceOptionSearch;
				}
			}//Next Option
		}//Next device
	}//Next adapter

EndWindowedDeviceOptionSearch:
	if (pBestOptions == NULL) return false;

	//Fill out settings details
	D3DSettings.windowed = true;
	pSettings = D3DSettings.GetSettings();
	pSettings->adapterOrdinal = pBestOptions->adapterOrdinal;
	pSettings->displayMode = displayMode;
	pSettings->deviceType = pBestOptions->deviceType;
	pSettings->backBufferFormat = pBestOptions->backBufferFormat;
	pSettings->depthStencilFormat = pBestOptions->depthFormats[0];
	pSettings->multisampleQuality = 0;
	pSettings->vertexProcessingType = pBestOptions->vertexProcessingTypes[0];
	pSettings->presentInterval = pBestOptions->presentIntervals[0];

	//found a mode
	return true;
}

bool CD3DInitialize::FindBestFullscreenMode(CD3DSettings & D3DSettings, D3DDISPLAYMODE * pMatchMode, bool bRequireHAL, bool bRequireREF)
{
	//Will try to match pMatchMode width, height, refresh rate and depth format. If no preference is passed it will try to match the current display mode
	ULONG i, j, k;
	D3DDISPLAYMODE adapterDisplayMode, bestAdapterDisplayMode, bestDisplayMode;
	CD3DEnumAdapter* pBestAdapter = nullptr;
	CD3DEnumDevice* pBestDevice = nullptr;
	CD3DEnumDeviceOptions* pBestOptions = nullptr;
	CD3DSettings::Settings* pSettings = nullptr;

	bestAdapterDisplayMode.Width = 0;
	bestAdapterDisplayMode.Height = 0;
	bestAdapterDisplayMode.Format = D3DFMT_UNKNOWN;
	bestAdapterDisplayMode.RefreshRate = 0;

	//Loop through each adapter
	for (i = 0; i < GetAdapterCount(); i++)
	{
		CD3DEnumAdapter* pAdapter = m_vpAdapters[i];
		//retrieve the desktop display mode
		m_pD3D->GetAdapterDisplayMode(pAdapter->ordinal, &adapterDisplayMode);
		//If any preferences werer passed, ovewrite to test for matches
		if (pMatchMode)
		{
			if (pMatchMode->Width != 0) adapterDisplayMode.Width = pMatchMode->Width;
			if (pMatchMode->Height != 0) adapterDisplayMode.Height = pMatchMode->Height;
			if (pMatchMode->RefreshRate != 0) adapterDisplayMode.RefreshRate = pMatchMode->RefreshRate;
			if (pMatchMode->Format != D3DFMT_UNKNOWN) adapterDisplayMode.Format = pMatchMode->Format;
		}
		//Loop through each device
		for (j = 0; j < pAdapter->devices.size(); j++)
		{
			CD3DEnumDevice* pDevice = pAdapter->devices[j];
			//Skip if this is not one of the requirementes
			if ((bRequireHAL && pDevice->DeviceType != D3DDEVTYPE_HAL) ||
				(bRequireREF && pDevice->DeviceType != D3DDEVTYPE_REF)) continue;

			//Loop through each option
			for (k = 0; k < pDevice->options.size(); k++)
			{
				CD3DEnumDeviceOptions* pOptions = pDevice->options[k];
				bool matchedBB = pOptions->backBufferFormat == pOptions->adapterFormat;
				bool matchedAdapterFormat = pOptions->adapterFormat == adapterDisplayMode.Format;
				
				if (pOptions->windowed) continue;

				//Check if that's the best device so far
				if (pBestOptions == nullptr ||//Don't have a best option yet
					(pBestOptions->deviceType != D3DDEVTYPE_HAL && pOptions->deviceType == D3DDEVTYPE_HAL) ||//Found the first HAL device
					(pOptions->deviceType == D3DDEVTYPE_HAL && pBestOptions->adapterFormat != adapterDisplayMode.Format && matchedAdapterFormat) ||//We found a HAL that matched the adapterFormat
					(pOptions->deviceType == D3DDEVTYPE_HAL && matchedAdapterFormat && matchedBB))//Perfect match
				{
					//Store best so far
					bestAdapterDisplayMode = adapterDisplayMode;
					pBestAdapter = pAdapter;
					pBestDevice = pDevice;
					pBestOptions = pOptions;

					if (pOptions->deviceType == D3DDEVTYPE_HAL && matchedAdapterFormat && matchedBB)
						goto EndFullScreenDeviceOptionSearch;
				}
			}//Nest option set
		}//Next device type
	}//Next adapter

EndFullScreenDeviceOptionSearch:

	if (pBestOptions == nullptr) return false;
	//Needs to find a display mode on the best adapter the uses pBestOptions->adapterFormat 
	//and is as close to bestAdapterDisplayMode's res as possible
	bestDisplayMode.Width = 0;
	bestDisplayMode.Height = 0;
	bestDisplayMode.Format = D3DFMT_UNKNOWN;
	bestDisplayMode.RefreshRate = 0;

	//Loop through valid display modes
	for (i = 0; i < pBestAdapter->modes.size(); i++)
	{
		D3DDISPLAYMODE mode = pBestAdapter->modes[i];
		//Skip if it doesn't match our best format
		if (mode.Format != pBestOptions->adapterFormat) continue;
		//Did we found a perfect match?
		if (mode.Width == bestAdapterDisplayMode.Width &&
			mode.Height == bestAdapterDisplayMode.Height &&
			mode.RefreshRate == bestAdapterDisplayMode.RefreshRate)
		{
			bestDisplayMode = mode;
			break;
		}
		else if (bestDisplayMode.Width == 0 || mode.Width == bestAdapterDisplayMode.Width)
			bestDisplayMode = mode;
	}//Next mode

	//Fill out passed settings details
	D3DSettings.windowed = false;
	pSettings = D3DSettings.GetSettings();
	pSettings->adapterOrdinal = pBestOptions->adapterOrdinal;
	pSettings->displayMode = bestDisplayMode;
	pSettings->deviceType = pBestOptions->deviceType;
	pSettings->backBufferFormat = pBestOptions->backBufferFormat;
	pSettings->depthStencilFormat = pBestOptions->depthFormats[0];
	pSettings->multiSampleType = pBestOptions->multiSampleTypes[0];
	pSettings->multisampleQuality = 0;
	pSettings->vertexProcessingType = pBestOptions->vertexProcessingTypes[0];
	pSettings->presentInterval = pBestOptions->presentIntervals[0];

	//Sucess
	return true;
}

D3DPRESENT_PARAMETERS CD3DInitialize::BuildPresentParameters(CD3DSettings & D3DSettings, ULONG flags)
{
	D3DPRESENT_PARAMETERS d3dpp;
	CD3DSettings::Settings* pSettings = D3DSettings.GetSettings();

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	//Fill out our common present parameters
	d3dpp.BackBufferCount;
	d3dpp.BackBufferFormat = pSettings->backBufferFormat;
	d3dpp.Windowed = D3DSettings.windowed;
	d3dpp.MultiSampleType = pSettings->multiSampleType;
	d3dpp.MultiSampleQuality = pSettings->multisampleQuality;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = pSettings->depthStencilFormat;
	d3dpp.PresentationInterval = pSettings->presentInterval;
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL | flags;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	//Is this fullscreen?
	if (!d3dpp.Windowed)
	{
		d3dpp.FullScreen_RefreshRateInHz = pSettings->displayMode.RefreshRate;
		d3dpp.BackBufferWidth = pSettings->displayMode.Width;
		d3dpp.BackBufferHeight = pSettings->displayMode.Height;
	}

	return d3dpp;
}

HRESULT CD3DInitialize::EnumerateAdapters()
{
	HRESULT hRet;

	//Store the number of available adapters
	ULONG nAdapterCount = m_pD3D->GetAdapterCount();

	for (ULONG i = 0; i < nAdapterCount; i++)
	{
		CD3DEnumAdapter* pAdapter = new CD3DEnumAdapter;
		if (!pAdapter) return E_OUTOFMEMORY;

		//----Fill pAdapter
		pAdapter->ordinal = i;
		m_pD3D->GetAdapterIdentifier(i, 0, &pAdapter->identifier);
		//Enumerate all display modes
		if (FAILED(hRet = EnumerateDisplayModes(pAdapter)) ||
			FAILED(hRet = EnumerateDevices(pAdapter)))
		{
			delete pAdapter;
			if (hRet == E_ABORT) continue; 
			else return hRet;
		}
		//Add this adapter to the list
		try
		{
			m_vpAdapters.push_back(pAdapter);
		}
		catch (...)
		{
			delete pAdapter;
			return E_OUTOFMEMORY;
		}
	}//Next adapter

	return S_OK;
}

HRESULT CD3DInitialize::EnumerateDisplayModes(CD3DEnumAdapter * pAdapter)
{
	HRESULT hRet;
	ULONG i, j;
	D3DDISPLAYMODE mode;

	//Loop through each valid 'Adapter' format
	for (i = 0; i < validAdapterFormatCount; i++)
	{
		//Retrieve the number of valid modes for ths format
		ULONG nModeCount = m_pD3D->GetAdapterModeCount(pAdapter->ordinal, validAdapterFormats[i]);
		if (nModeCount == 0) continue;

		//Loop through each display mode for this format
		for (j = 0; j < nModeCount; j++)
		{
			//Retrieve the display mode
			hRet = m_pD3D->EnumAdapterModes(pAdapter->ordinal, validAdapterFormats[i], j, &mode);
			if (FAILED(hRet)) return hRet;

			//Is supported by the game?
			if (!ValidateDisplayMode(mode)) continue;
			//Add this mode to the adapter
			try	{ pAdapter->modes.push_back(mode); }
			catch (...) { return E_OUTOFMEMORY; }
		}//Next adapter mode
	}//Next adapter format

	//Sucess?
	return (pAdapter->modes.size() == 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumerateDevices(CD3DEnumAdapter * pAdapter)
{
	ULONG i;
	HRESULT hRet;
	D3DCAPS9 caps;

	for (i = 0; i < deviceTypeCount; i++)
	{
		//Retrieve device caps(on failure, device note generally available
		if (FAILED(m_pD3D->GetDeviceCaps(pAdapter->ordinal, deviceTypes[i], &caps))) continue;

		//Supported by the game
		if (!ValidateDevice(deviceTypes[i], caps)) continue;

		//Allocate new device
		CD3DEnumDevice* pDevice = new CD3DEnumDevice;
		if(!pDevice) return E_OUTOFMEMORY;

		//Store device information
		pDevice->DeviceType = deviceTypes[i];
		pDevice->caps = caps;

		//Retrieve various init options for this device
		if (FAILED(hRet = EnumerateDeviceOptions(pDevice, pAdapter)))
		{
			delete pDevice;
			if (hRet == E_ABORT) continue;
			else return hRet;
		}

		try { pAdapter->devices.push_back(pDevice); } 
		catch (...) { delete pDevice; return E_OUTOFMEMORY; }

		//Sucess?
		return (pAdapter->devices.size() == 0) ? E_ABORT : S_OK;
	}
}

HRESULT CD3DInitialize::EnumerateDeviceOptions(CD3DEnumDevice * pDevice, CD3DEnumAdapter * pAdapter)
{
	HRESULT hRet;
	ULONG i, j, k;
	bool windowed;
	D3DFORMAT adapterFormats[validAdapterFormatCount];
	ULONG adapterFormatCount = 0;
	D3DFORMAT adapterFormat, backbufferFormat;

	//Build a list of all format used by the adapter
	for (i = 0; i < pAdapter->modes.size(); i++)
	{
		//Already added to the list?
		for (j = 0; j < adapterFormatCount; j++)
			if (pAdapter->modes[i].Format == adapterFormats[j]) break;

		//Add it to the list if not existing
		if (j == adapterFormatCount) adapterFormats[adapterFormatCount++] = pAdapter->modes[i].Format;
	}//Next adapter mode
	for (i = 0; i < adapterFormatCount; i++)
	{
		adapterFormat = adapterFormats[i];
		//Looop through all valid back buffer formats
		for (j = 0; j < backBufferFormatCount; j++)
		{
			backbufferFormat = backBufferFormats[j];

			//Test windowed/Fullscreen modes
			for (k = 0; k < 2; k++)
			{
				windowed = k == 1;//First full screen, then windowed mode
				//Skip if this is not a valid device type
				if (FAILED(m_pD3D->CheckDeviceType(pAdapter->ordinal, pDevice->DeviceType, adapterFormat, backbufferFormat, windowed)))
					continue;

				//Is this option supported by the game?
				if (!ValidateDeviceOptions(backbufferFormat, windowed)) continue;

				//Allocate a new device options set
				CD3DEnumDeviceOptions* pDeviceOptions = new CD3DEnumDeviceOptions;
				if (!pDeviceOptions) return E_OUTOFMEMORY;

				//Store device options detail
				pDeviceOptions->adapterOrdinal = pAdapter->ordinal;
				pDeviceOptions->deviceType = pDevice->DeviceType;
				pDeviceOptions->adapterFormat = adapterFormat;
				pDeviceOptions->backBufferFormat = backbufferFormat;
				pDeviceOptions->caps = pDevice->caps;
				pDeviceOptions->windowed = windowed;

				//Enumerate the various options components
				if (FAILED(hRet = EnumerateDepthStencilFormats(pDeviceOptions)) ||
					FAILED(hRet = EnumerateMultiSampleTypes(pDeviceOptions)) ||
					FAILED(hRet = EnumerateVertexProcessingTypes(pDeviceOptions)) ||
					FAILED(hRet = EnumeratePresentIntervals(pDeviceOptions)))
				{
					delete pDeviceOptions;
					if (hRet == E_ABORT) continue; else return hRet;
				}

				//Add this to our device
				try { pDevice->options.push_back(pDeviceOptions); }
				catch (...) { delete pDeviceOptions; return E_OUTOFMEMORY; }
			}//Next windowed state
		}//Next Backbuffer Format
	}//Next Adapter Format

	//Sucess?
	return (pDevice->options.size() == 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumerateDepthStencilFormats(CD3DEnumDeviceOptions * pDeviceOptions)
{
	ULONG i;

	try
	{
		//Loop through each depth stencil format
		for (i = 0; i < depthStencilFormatCount; i++)
		{
			//Test to see if this is a valid depth surface format
			if (SUCCEEDED(m_pD3D->CheckDeviceFormat(
				pDeviceOptions->adapterOrdinal,
				pDeviceOptions->deviceType,
				pDeviceOptions->adapterFormat,
				D3DUSAGE_DEPTHSTENCIL,
				D3DRTYPE_SURFACE,
				depthStencilFormats[i])))
			{
				//Test to see if this is a valid depth/stencil format for this mode
				if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(
					pDeviceOptions->adapterOrdinal,
					pDeviceOptions->deviceType,
					pDeviceOptions->adapterFormat,
					pDeviceOptions->backBufferFormat,
					depthStencilFormats[i])))
				{
					//Is this supported by the user?
					if (ValidateDepthStencilFormat(depthStencilFormats[i]))
					{
						//Add this as a valid depthstencil format
						pDeviceOptions->depthFormats.push_back(depthStencilFormats[i]);
					}
				}//End if valid for this mode
			}//end if valid depthstencil format
		}//Next depth stencil format
	}//End try block
	catch (...) { return E_OUTOFMEMORY; }

	//Sucess?
	return (pDeviceOptions->depthFormats.size() == 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumerateMultiSampleTypes(CD3DEnumDeviceOptions * pDeviceOptions)
{
	ULONG i, quality;

	try
	{
		//Loop through each multi-sample types
		for (i = 0; i < multiSampleTypeCount; i++)
		{
			//Is this supported by the game
			if (!ValidateMultiSampleType(multiSampleTypes[i])) continue;
			//Check if this multi-sample type is supported
			if (SUCCEEDED(
				m_pD3D->CheckDeviceMultiSampleType(
				pDeviceOptions->adapterOrdinal,
				pDeviceOptions->deviceType,
				pDeviceOptions->backBufferFormat,
				pDeviceOptions->windowed,
				multiSampleTypes[i],
				&quality)))
			{
				pDeviceOptions->multiSampleTypes.push_back(multiSampleTypes[i]);
				pDeviceOptions->multiSampleQuality.push_back(quality);
			}
		}
	}
	catch (...) { return E_OUTOFMEMORY; }

	//Sucess?
	return (pDeviceOptions->multiSampleTypes.size() == 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumerateVertexProcessingTypes(CD3DEnumDeviceOptions * pDeviceOptions)
{
	try
	{
		if (pDeviceOptions->caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)//Supports T&L?
		{
			if (pDeviceOptions->caps.DevCaps & D3DDEVCAPS_PUREDEVICE)//can be pure device
			{
				if (ValidateVertexProcessingType(PURE_HARDWARE_VP))//Supported by the game
					pDeviceOptions->vertexProcessingTypes.push_back(PURE_HARDWARE_VP);
			}
			if (ValidateVertexProcessingType(HARDWARE_VP))//T&L and mixed vp?
				pDeviceOptions->vertexProcessingTypes.push_back(HARDWARE_VP);
			if (ValidateVertexProcessingType(MIXED_VP))
				pDeviceOptions->vertexProcessingTypes.push_back(MIXED_VP);
		}//end if T&L
		//Alway supports software
		if (ValidateVertexProcessingType(SOFTWARE_VP))
			pDeviceOptions->vertexProcessingTypes.push_back(SOFTWARE_VP);
	}
	catch (...) { return E_OUTOFMEMORY; }
	//Sucess?
	return (pDeviceOptions->vertexProcessingTypes.size() == 0) ? E_ABORT : S_OK;
}

HRESULT CD3DInitialize::EnumeratePresentIntervals(CD3DEnumDeviceOptions * pDeviceOptions)
{
	ULONG i, interval;

	try
	{
		for (i = 0; i < presentIntervalCount; i++)
		{
			interval = presentIntervals[i];

			//Default is always available, always add
			if (interval == D3DPRESENT_INTERVAL_DEFAULT)
			{
				pDeviceOptions->presentIntervals.push_back(interval);
				continue;
			}

			//If not supported by game, continue
			if (!ValidatePresentInterval(interval)) continue;

			//If device is windowed, skip anything above one
			if (pDeviceOptions->windowed &&
				(interval == D3DPRESENT_INTERVAL_TWO || interval == D3DPRESENT_INTERVAL_THREE || interval == D3DPRESENT_INTERVAL_FOUR)) continue;

			//Supported by the device options combo?
			if (pDeviceOptions->caps.PresentationIntervals & interval)
				pDeviceOptions->presentIntervals.push_back(interval);
		}//Next interval type
	}//End try block
	catch (...) { return E_OUTOFMEMORY; }

	//Sucess?
	return (pDeviceOptions->presentIntervals.size() == 0) ? E_ABORT : S_OK;
}

LPDIRECT3DDEVICE9 CD3DInitialize::GetDirect3DDevice()
{
	if (!m_pD3DDevice) return nullptr;
	//Add ref on the device
	m_pD3DDevice->AddRef();

	//We've duplicated the pointer
	return m_pD3DDevice;
}

