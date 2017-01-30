#include "D3DSettingsDlg.h"
#include "resource1.h"
#include <string>

//-----------------------------------
//Local Module Functions Declarations
//-----------------------------------

LPCTSTR D3DDevTypeToString(D3DDEVTYPE devType);
LPCTSTR MultisampleTypeToString(D3DMULTISAMPLE_TYPE multiSampleType);
LPCTSTR VertexProcessingTypeToString(VERTEXPROCESSING_TYPE vpt);
LPCTSTR D3DFormatToString(D3DFORMAT format, bool bWithPrefix = true);
LPCTSTR PresentIntervalToString(UINT presentInterval);
LPCTSTR ConvertToLPCTSTR(char* string);

LPCTSTR D3DDevTypeToString(D3DDEVTYPE devType)
{
	switch (devType)
	{
	case D3DDEVTYPE_HAL: return L"D3DDEVTYPE_HAL";
	case D3DDEVTYPE_SW: return L"D3DDEVTYPE_SW";
	case D3DDEVTYPE_REF: return L"D3DDEVTYPE_REF";
	default: return L"Unknown D3DDEVTYPE";
	}
}

LPCTSTR MultisampleTypeToString(D3DMULTISAMPLE_TYPE multiSampleType)
{
	switch (multiSampleType)
	{
	case D3DMULTISAMPLE_NONE:   return L"D3DMULTISAMPLE_NONE";
	case D3DMULTISAMPLE_NONMASKABLE: return L"D3DMULTISAMPLE_NONMASKABLE";
	case D3DMULTISAMPLE_2_SAMPLES: return L"D3DMULTISAMPLE_2_SAMPLES";
	case D3DMULTISAMPLE_3_SAMPLES: return L"D3DMULTISAMPLE_3_SAMPLES";
	case D3DMULTISAMPLE_4_SAMPLES: return L"D3DMULTISAMPLE_4_SAMPLES";
	case D3DMULTISAMPLE_5_SAMPLES: return L"D3DMULTISAMPLE_5_SAMPLES";
	case D3DMULTISAMPLE_6_SAMPLES: return L"D3DMULTISAMPLE_6_SAMPLES";
	case D3DMULTISAMPLE_7_SAMPLES: return L"D3DMULTISAMPLE_7_SAMPLES";
	case D3DMULTISAMPLE_8_SAMPLES: return L"D3DMULTISAMPLE_8_SAMPLES";
	case D3DMULTISAMPLE_9_SAMPLES: return L"D3DMULTISAMPLE_9_SAMPLES";
	case D3DMULTISAMPLE_10_SAMPLES: return L"D3DMULTISAMPLE_10_SAMPLES";
	case D3DMULTISAMPLE_11_SAMPLES: return L"D3DMULTISAMPLE_11_SAMPLES";
	case D3DMULTISAMPLE_12_SAMPLES: return L"D3DMULTISAMPLE_12_SAMPLES";
	case D3DMULTISAMPLE_13_SAMPLES: return L"D3DMULTISAMPLE_13_SAMPLES";
	case D3DMULTISAMPLE_14_SAMPLES: return L"D3DMULTISAMPLE_14_SAMPLES";
	case D3DMULTISAMPLE_15_SAMPLES: return L"D3DMULTISAMPLE_15_SAMPLES";
	case D3DMULTISAMPLE_16_SAMPLES: return L"D3DMULTISAMPLE_16_SAMPLES";
	default:                    return L"Unknown D3DMULTISAMPLE_TYPE";
	}
}

LPCTSTR VertexProcessingTypeToString(VERTEXPROCESSING_TYPE vpt)
{
	switch (vpt)
	{
	case SOFTWARE_VP:      return L"SOFTWARE_VP";
	case MIXED_VP:         return L"MIXED_VP";
	case HARDWARE_VP:      return L"HARDWARE_VP";
	case PURE_HARDWARE_VP: return L"PURE_HARDWARE_VP";
	default:               return L"Unknown VERTEXPROCESSING_TYPE";
	}
}

LPCTSTR PresentIntervalToString(UINT pi)
{
	switch (pi)
	{
	case D3DPRESENT_INTERVAL_IMMEDIATE: return L"D3DPRESENT_INTERVAL_IMMEDIATE";
	case D3DPRESENT_INTERVAL_DEFAULT:   return L"D3DPRESENT_INTERVAL_DEFAULT";
	case D3DPRESENT_INTERVAL_ONE:       return L"D3DPRESENT_INTERVAL_ONE";
	case D3DPRESENT_INTERVAL_TWO:       return L"D3DPRESENT_INTERVAL_TWO";
	case D3DPRESENT_INTERVAL_THREE:     return L"D3DPRESENT_INTERVAL_THREE";
	case D3DPRESENT_INTERVAL_FOUR:      return L"D3DPRESENT_INTERVAL_FOUR";
	default:                            return L"Unknown Present-Interval";
	}
}

LPCTSTR D3DFormatToString(D3DFORMAT format, bool bWithPrefix)
{
	LPTSTR pstr = NULL;

	switch (format)
	{
	case D3DFMT_UNKNOWN:         pstr = L"D3DFMT_UNKNOWN"; break;
	case D3DFMT_R8G8B8:          pstr = L"D3DFMT_R8G8B8"; break;
	case D3DFMT_A8R8G8B8:        pstr = L"D3DFMT_A8R8G8B8"; break;
	case D3DFMT_X8R8G8B8:        pstr = L"D3DFMT_X8R8G8B8"; break;
	case D3DFMT_R5G6B5:          pstr = L"D3DFMT_R5G6B5"; break;
	case D3DFMT_X1R5G5B5:        pstr = L"D3DFMT_X1R5G5B5"; break;
	case D3DFMT_A1R5G5B5:        pstr = L"D3DFMT_A1R5G5B5"; break;
	case D3DFMT_A4R4G4B4:        pstr = L"D3DFMT_A4R4G4B4"; break;
	case D3DFMT_R3G3B2:          pstr = L"D3DFMT_R3G3B2"; break;
	case D3DFMT_A8:              pstr = L"D3DFMT_A8"; break;
	case D3DFMT_A8R3G3B2:        pstr = L"D3DFMT_A8R3G3B2"; break;
	case D3DFMT_X4R4G4B4:        pstr = L"D3DFMT_X4R4G4B4"; break;
	case D3DFMT_A2B10G10R10:     pstr = L"D3DFMT_A2B10G10R10"; break;
	case D3DFMT_A8B8G8R8:        pstr = L"D3DFMT_A8B8G8R8"; break;
	case D3DFMT_X8B8G8R8:        pstr = L"D3DFMT_X8B8G8R8"; break;
	case D3DFMT_G16R16:          pstr = L"D3DFMT_G16R16"; break;
	case D3DFMT_A2R10G10B10:     pstr = L"D3DFMT_A2R10G10B10"; break;
	case D3DFMT_A16B16G16R16:    pstr = L"D3DFMT_A16B16G16R16"; break;
	case D3DFMT_A8P8:            pstr = L"D3DFMT_A8P8"; break;
	case D3DFMT_P8:              pstr = L"D3DFMT_P8"; break;
	case D3DFMT_L8:              pstr = L"D3DFMT_L8"; break;
	case D3DFMT_A8L8:            pstr = L"D3DFMT_A8L8"; break;
	case D3DFMT_A4L4:            pstr = L"D3DFMT_A4L4"; break;
	case D3DFMT_V8U8:            pstr = L"D3DFMT_V8U8"; break;
	case D3DFMT_L6V5U5:          pstr = L"D3DFMT_L6V5U5"; break;
	case D3DFMT_X8L8V8U8:        pstr = L"D3DFMT_X8L8V8U8"; break;
	case D3DFMT_Q8W8V8U8:        pstr = L"D3DFMT_Q8W8V8U8"; break;
	case D3DFMT_V16U16:          pstr = L"D3DFMT_V16U16"; break;
	case D3DFMT_A2W10V10U10:     pstr = L"D3DFMT_A2W10V10U10"; break;
	case D3DFMT_UYVY:            pstr = L"D3DFMT_UYVY"; break;
	case D3DFMT_YUY2:            pstr = L"D3DFMT_YUY2"; break;
	case D3DFMT_DXT1:            pstr = L"D3DFMT_DXT1"; break;
	case D3DFMT_DXT2:            pstr = L"D3DFMT_DXT2"; break;
	case D3DFMT_DXT3:            pstr = L"D3DFMT_DXT3"; break;
	case D3DFMT_DXT4:            pstr = L"D3DFMT_DXT4"; break;
	case D3DFMT_DXT5:            pstr = L"D3DFMT_DXT5"; break;
	case D3DFMT_D16_LOCKABLE:    pstr = L"D3DFMT_D16_LOCKABLE"; break;
	case D3DFMT_D32:             pstr = L"D3DFMT_D32"; break;
	case D3DFMT_D15S1:           pstr = L"D3DFMT_D15S1"; break;
	case D3DFMT_D24S8:           pstr = L"D3DFMT_D24S8"; break;
	case D3DFMT_D24X8:           pstr = L"D3DFMT_D24X8"; break;
	case D3DFMT_D24X4S4:         pstr = L"D3DFMT_D24X4S4"; break;
	case D3DFMT_D16:             pstr = L"D3DFMT_D16"; break;
	case D3DFMT_L16:             pstr = L"D3DFMT_L16"; break;
	case D3DFMT_VERTEXDATA:      pstr = L"D3DFMT_VERTEXDATA"; break;
	case D3DFMT_INDEX16:         pstr = L"D3DFMT_INDEX16"; break;
	case D3DFMT_INDEX32:         pstr = L"D3DFMT_INDEX32"; break;
	case D3DFMT_Q16W16V16U16:    pstr = L"D3DFMT_Q16W16V16U16"; break;
	case D3DFMT_MULTI2_ARGB8:    pstr = L"D3DFMT_MULTI2_ARGB8"; break;
	case D3DFMT_R16F:            pstr = L"D3DFMT_R16F"; break;
	case D3DFMT_G16R16F:         pstr = L"D3DFMT_G16R16F"; break;
	case D3DFMT_A16B16G16R16F:   pstr = L"D3DFMT_A16B16G16R16F"; break;
	case D3DFMT_R32F:            pstr = L"D3DFMT_R32F"; break;
	case D3DFMT_G32R32F:         pstr = L"D3DFMT_G32R32F"; break;
	case D3DFMT_A32B32G32R32F:   pstr = L"D3DFMT_A32B32G32R32F"; break;
	case D3DFMT_CxV8U8:          pstr = L"D3DFMT_CxV8U8"; break;
	default:                     pstr = L"Unknown format"; break;
	}

	if (bWithPrefix || _tcsstr(pstr, L"D3DFMT_") == NULL)
		return pstr;
	else
		return pstr + lstrlen(L"D3DFMT_");
}

LPCTSTR ConvertToLPCTSTR(const char* string)
{
	ULONG size = 0, i = 0;
	while (string[i] != 0)
		i++;

	size = i + 1;

	wchar_t* newString = new wchar_t[size];

	for (i = 0; i < size; i++)
		newString[i] = string[i];

	return (LPCTSTR)newString;
}

CD3DSettingsDlg::CD3DSettingsDlg()
{
	m_hWndDlg = nullptr;
	m_pInitialize = nullptr;
}

CD3DSettingsDlg::~CD3DSettingsDlg()
{
	//Clean up
}

int CD3DSettingsDlg::ShowDialog(CD3DInitialize* pInitialize, CD3DSettings* pSettings, HWND hWndParent)
{
	m_pInitialize = pInitialize;
	//Store the settings passed in
	if (pSettings) m_D3DSettings = *pSettings;
	else
	{
		ZeroMemory(&m_D3DSettings, sizeof(CD3DSettings));
		m_D3DSettings.windowed = true;
		//Setup windowed mode defaults
		D3DDISPLAYMODE displayMode;
		m_pInitialize->GetDirect3D()->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

		GetSettings()->adapterOrdinal = D3DADAPTER_DEFAULT;
		GetSettings()->displayMode = displayMode;
	}
	//Create dialog box
	return DialogBoxParam(nullptr, MAKEINTRESOURCE(IDD_D3DSETTINGS), hWndParent, StaticDlgProc, (LPARAM) this);
}

//------------------------------------------------------
// This is the main message pump for all settings dialogs, 
// it captures the appropriate messages and routes them through to the settings
// class for which it was intended, therefore givving full class access
//------------------------------------------------------
BOOL CD3DSettingsDlg::StaticDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	//If this is a create message, trap the 'this' pointer passed in and store it within the window
	if (Message == WM_INITDIALOG) SetWindowLong(hWnd, GWL_USERDATA, lParam);//Make us able to pass messages back to the settings object
	//Obtain the correct destination for this mesage
	CD3DSettingsDlg* destination = (CD3DSettingsDlg*)GetWindowLong(hWnd, GWL_USERDATA);
	//If the hWnd has a related class, pass it through
	if (destination) return destination->SettingsDlgProc(hWnd, Message, wParam, lParam);
	//no destination found, defer to system...
	return FALSE;
}

//------------------------------------------------------
// Handle the messages for the settings dialog
//------------------------------------------------------
BOOL CD3DSettingsDlg::SettingsDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	ULONG i;

	switch (Message)
	{
	case WM_INITDIALOG:
		//Store the handle to our dialog
		m_hWndDlg = hWnd;
		//Fill the adapter combo box
		for (i = 0; i < m_pInitialize->GetAdapterCount(); i++)
		{
			const CD3DEnumAdapter* pAdapter = m_pInitialize->GetAdapter(i);
			//Add to the combo box
			Combo_ItemAdd(IDC_COMBO_ADAPTER, (LPVOID)pAdapter, ConvertToLPCTSTR(pAdapter->identifier.Description));
			//If this is a setting we already have set, select it in the combo
			if (pAdapter->ordinal == GetSettings()->adapterOrdinal)
				Combo_SelectItemData(IDC_COMBO_ADAPTER, (LPVOID)pAdapter);
		}
		//If nothing is already selected, select the first item
		if (Combo_GetSelectedItemData(IDC_COMBO_ADAPTER) == nullptr && Combo_GetItemCount(IDC_COMBO_ADAPTER) > 0)
			Combo_SelectItem(IDC_COMBO_ADAPTER, 0);

		return TRUE;
		
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hWnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hWnd, IDCANCEL);
			break;
		case IDC_COMBO_ADAPTER:
			if (CBN_SELCHANGE == HIWORD(wParam)) AdapterChanged();
			break;
		case IDC_COMBO_DEVICE:
			if (CBN_SELCHANGE == HIWORD(wParam)) DeviceChanged();
			break;
		case IDC_COMBO_AFORMAT:
			if (CBN_SELCHANGE == HIWORD(wParam)) AdapterFormatChanged();
			break;
		case IDC_COMBO_RESOLUTION:
			if (CBN_SELCHANGE == HIWORD(wParam)) ResolutionChanged();
			break;
		case IDC_COMBO_REFRESH:
			if (CBN_SELCHANGE == HIWORD(wParam)) RefreshRateChanged();
			break;
		case IDC_COMBO_BBFORMAT:
			if (CBN_SELCHANGE == HIWORD(wParam)) BackBufferFormatChanged();
			break;
		case IDC_COMBO_DSFORMAT:
			if (CBN_SELCHANGE == HIWORD(wParam)) DepthStencilFormatChanged();
			break;
		case IDC_COMBO_MSTYPE:
			if (CBN_SELCHANGE == HIWORD(wParam)) MultisampleTypeChanged();
			break;
		case IDC_COMBO_MSQUALITY:
			if (CBN_SELCHANGE == HIWORD(wParam)) MultisampleQualityChanged();
			break;
		case IDC_COMBO_VPTYPE:
			if (CBN_SELCHANGE == HIWORD(wParam)) VertexProcessingChanged();
			break;
		case IDC_COMBO_PINTERVAL:
			if (CBN_SELCHANGE == HIWORD(wParam)) PresentIntervalChanged();
			break;
		case IDC_RADIO_WINDOWED:
		case IDC_RADIO_FULLSCREEN:
			WindowedFullscreenChanged();
			break;
		}//End control switch
		break;
	default://Message not processed
		return FALSE;
	}//End switch message

	//Message processed
	return TRUE;
}

void CD3DSettingsDlg::AdapterChanged()
{
	CD3DEnumAdapter* pAdapter = (CD3DEnumAdapter*)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER);
	if (pAdapter == nullptr) return;
	//Set the current adapter information
	GetSettings()->adapterOrdinal = pAdapter->ordinal;
	//Update windowed display mode settings
	if (m_D3DSettings.windowed == true)
	{
		D3DDISPLAYMODE displayMode;
		m_pInitialize->GetDirect3D()->GetAdapterDisplayMode(pAdapter->ordinal, &displayMode);
		GetSettings()->displayMode = displayMode;
	}

	//Update device combo box
	Combo_Clear(IDC_COMBO_DEVICE);
	for (ULONG i = 0; i < pAdapter->devices.size(); i++)
	{
		CD3DEnumDevice* pDevice = pAdapter->devices[i];
		//Add to the device combo
		Combo_ItemAdd(IDC_COMBO_DEVICE, pDevice, D3DDevTypeToString(pDevice->DeviceType));
		//If this is a settings we already have set, select it in the combo
		if (pDevice->DeviceType == GetSettings()->deviceType)
			Combo_SelectItemData(IDC_COMBO_DEVICE, pDevice);
	}//Next device

	//If nothing is already selected, select the first item
	if (Combo_GetSelectedItemData(IDC_COMBO_DEVICE) == nullptr && Combo_GetItemCount(IDC_COMBO_DEVICE) > 0)
		Combo_SelectItem(IDC_COMBO_DEVICE, 0);
}

void CD3DSettingsDlg::DeviceChanged()
{
	CD3DEnumDevice* pDevice = (CD3DEnumDevice*)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);
	if (pDevice == nullptr) return;

	GetSettings()->deviceType = pDevice->DeviceType;

	//Update fullscreen/windowed radio buttons
	bool hasWindowedDeviceOption = false;
	bool hasFullScreenDeviceOption = false;

	for (ULONG i = 0; i < pDevice->options.size(); i++)
	{
		CD3DEnumDeviceOptions* pDeviceOptions = pDevice->options[i];
		if (pDeviceOptions->windowed) hasWindowedDeviceOption = true;
		else hasFullScreenDeviceOption = true;
	}//Next device

	//Enable/Disable radio buttons
	EnableWindow(GetDlgItem(m_hWndDlg, IDC_RADIO_WINDOWED), hasWindowedDeviceOption);
	EnableWindow(GetDlgItem(m_hWndDlg, IDC_RADIO_FULLSCREEN), hasFullScreenDeviceOption);

	//If this i a setting we already have set, set the radio button
	if (m_D3DSettings.windowed && hasWindowedDeviceOption)
		CheckRadioButton(m_hWndDlg, IDC_RADIO_WINDOWED, IDC_RADIO_FULLSCREEN, IDC_RADIO_WINDOWED);
	else
		CheckRadioButton(m_hWndDlg, IDC_RADIO_WINDOWED, IDC_RADIO_FULLSCREEN, IDC_RADIO_FULLSCREEN);

	//Trigger the change
	WindowedFullscreenChanged();
}

//------------------------------------------------------
// Triggered when a change of windowed/fullscreen mode is posted
//------------------------------------------------------
void CD3DSettingsDlg::WindowedFullscreenChanged()
{
	CD3DEnumAdapter* pAdapter = (CD3DEnumAdapter*)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER);
	CD3DEnumDevice* pDevice = (CD3DEnumDevice*)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);
	if (pAdapter == nullptr || pDevice == nullptr) return;

	//Which mode is selected?
	if (IsDlgButtonChecked(m_hWndDlg, IDC_RADIO_WINDOWED))
	{
		m_D3DSettings.windowed = true;
		GetSettings()->adapterOrdinal = pAdapter->ordinal;
		GetSettings()->deviceType = pDevice->DeviceType;

		//Update adapter format combo box
		D3DFORMAT format = GetSettings()->displayMode.Format;
		Combo_Clear(IDC_COMBO_AFORMAT);
		Combo_ItemAdd(IDC_COMBO_AFORMAT, (LPVOID)format, D3DFormatToString(format));
		Combo_SelectItem(IDC_COMBO_AFORMAT, 0);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_AFORMAT), false);

		//Update resolution combo box
		ULONG ulResolutionDate, width, height;
		TCHAR strResolution[50];

		width = GetSettings()->displayMode.Width;
		height = GetSettings()->displayMode.Height;
		ulResolutionDate = MAKELONG(width, height);
		_sntprintf_s(strResolution, 50, L"%d x %d", width, height);
		strResolution[49] = 0;

		Combo_Clear(IDC_COMBO_RESOLUTION);
		Combo_ItemAdd(IDC_COMBO_RESOLUTION, (LPVOID)ulResolutionDate, (LPCTSTR)strResolution);
		Combo_SelectItem(IDC_COMBO_RESOLUTION, 0);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_RESOLUTION), false);

		//Update refresh rate combo box
		TCHAR strRefreshRate[50];
		ULONG ulRefreshRate = GetSettings()->displayMode.RefreshRate;

		_sntprintf_s(strRefreshRate, 50, L"%d Hz", ulRefreshRate);
		strRefreshRate[49] = 0;

		Combo_Clear(IDC_COMBO_REFRESH);
		Combo_ItemAdd(IDC_COMBO_REFRESH, (LPVOID)ulRefreshRate, (wchar_t*) strRefreshRate);
		Combo_SelectItem(IDC_COMBO_REFRESH, 0);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_REFRESH), false);
	}//End if windowed mode
	else//Full screen
	{
		m_D3DSettings.windowed = false;
		GetSettings()->adapterOrdinal = pAdapter->ordinal;
		GetSettings()->deviceType = pDevice->DeviceType;

		//Update adapter format combo boxx
		Combo_Clear(IDC_COMBO_AFORMAT);
		for (ULONG i = 0; i < pDevice->options.size(); i++)
		{
			CD3DEnumDeviceOptions* pOptions = pDevice->options[i];
			D3DFORMAT adapterFormat = pOptions->adapterFormat;

			//Determine if this has already been added
			if (!Combo_FindText(IDC_COMBO_AFORMAT, D3DFormatToString(adapterFormat)))
			{
				//Not added yet, add now
				Combo_ItemAdd(IDC_COMBO_AFORMAT, (LPVOID)adapterFormat, D3DFormatToString(adapterFormat));
				//If this setting was already set, check in the combo
				if (adapterFormat == GetSettings()->displayMode.Format)
					Combo_SelectItemData(IDC_COMBO_AFORMAT, (LPVOID)adapterFormat);
			}
		}//Next format

		//If nothing is already selecte, select the frist item
		if (Combo_GetSelectedItemData(IDC_COMBO_AFORMAT) == nullptr && Combo_GetItemCount(IDC_COMBO_AFORMAT) > 0)
			Combo_SelectItem(IDC_COMBO_AFORMAT, 0);

		//Enable relevant combo boxes
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_AFORMAT), true);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_RESOLUTION), true);
		EnableWindow(GetDlgItem(m_hWndDlg, IDC_COMBO_REFRESH), true);
	}//End if fullscreen
}

void CD3DSettingsDlg::AdapterFormatChanged()
{
	ULONG i;
	D3DFORMAT adapterFormat;

	//Is fullscreen selected?
	if (!m_D3DSettings.windowed)
	{
		CD3DEnumAdapter* pAdapter = (CD3DEnumAdapter*)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER);
		if (pAdapter == nullptr) return;
		adapterFormat = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_AFORMAT));
		//Set the current device information
		GetSettings()->displayMode.Format = adapterFormat;

		//Update resolution combo boxx
		Combo_Clear(IDC_COMBO_RESOLUTION);
		for (i = 0; i < pAdapter->modes.size(); i++)
		{
			D3DDISPLAYMODE mode = pAdapter->modes[i];
			//If the formats match
			if (mode.Format = adapterFormat)
			{
				//Build resolution data
				ULONG ulResolutionData;
				TCHAR strResolution[50];

				ulResolutionData = MAKELONG(mode.Width, mode.Height);
				_sntprintf_s(strResolution, 50, L"%d x %d", mode.Width, mode.Height);
				strResolution[49] = 0;
				//Deterime inf this item already exists
				if (!Combo_FindText(IDC_COMBO_RESOLUTION, strResolution))
				{
					Combo_ItemAdd(IDC_COMBO_RESOLUTION, (LPVOID)ulResolutionData, (LPCTSTR)strResolution);
					if (GetSettings()->displayMode.Width == mode.Width && GetSettings()->displayMode.Height == mode.Height)
						Combo_SelectItemData(IDC_COMBO_RESOLUTION, (LPVOID)ulResolutionData);
				}
			}//End if matching formats
		}//Next adapter display mode
		//If nothing is already selected, select the first item
		if (Combo_GetSelectedItemData(IDC_COMBO_RESOLUTION) == NULL && Combo_GetItemCount(IDC_COMBO_RESOLUTION) > 0)
			Combo_SelectItem(IDC_COMBO_RESOLUTION, 0);
	}//End if fullscreen mode

	//Update backbuffer format combo box
	CD3DEnumDevice* pDevice = (CD3DEnumDevice*)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);	
	Combo_Clear(IDC_COMBO_BBFORMAT);

	for (i = 0; i < pDevice->options.size(); i++)
	{
		CD3DEnumDeviceOptions* pOptions = pDevice->options[i];
		if (pOptions->windowed == m_D3DSettings.windowed && pOptions->adapterFormat == GetSettings()->displayMode.Format)
		{
			if (!Combo_FindText(IDC_COMBO_BBFORMAT, D3DFormatToString(pOptions->backBufferFormat)))
			{
				// Add the format
				Combo_ItemAdd(IDC_COMBO_BBFORMAT, (LPVOID)pOptions->backBufferFormat, D3DFormatToString(pOptions->backBufferFormat));

				// If this is a setting we already have set, select it in the combo
				if (GetSettings()->backBufferFormat == pOptions->backBufferFormat)
					Combo_SelectItemData(IDC_COMBO_BBFORMAT, (LPVOID)pOptions->backBufferFormat);
			} // End if No Text Found
		} // End if matches
	} // Next Option

	// If nothing is already selected, select the first item
	if (Combo_GetSelectedItemData(IDC_COMBO_BBFORMAT) == NULL && Combo_GetItemCount(IDC_COMBO_BBFORMAT) > 0)
		Combo_SelectItem(IDC_COMBO_BBFORMAT, 0);
}

void CD3DSettingsDlg::ResolutionChanged()
{
	ULONG i;

	if (m_D3DSettings.windowed) return;

	// Retrieve the selected adapter
	CD3DEnumAdapter * pAdapter = (CD3DEnumAdapter*)Combo_GetSelectedItemData(IDC_COMBO_ADAPTER);
	if (pAdapter == NULL) return;

	// Update settings with new resolution
	ULONG ulResolutionData = (ULONG)Combo_GetSelectedItemData(IDC_COMBO_RESOLUTION);
	ULONG Width = LOWORD(ulResolutionData);
	ULONG Height = HIWORD(ulResolutionData);
	GetSettings()->displayMode.Width = Width;
	GetSettings()->displayMode.Height = Height;

	// Update refresg rate list based on new resolution
	D3DFORMAT AFormat = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_AFORMAT));
	Combo_Clear(IDC_COMBO_REFRESH);
	for (i = 0; i < pAdapter->modes.size(); i++)
	{
		D3DDISPLAYMODE Mode = pAdapter->modes[i];
		// Does the mode match ?
		if (Mode.Format == AFormat && Mode.Width == Width && Mode.Height == Height)
		{
			TCHAR strRefreshRate[50];
			_sntprintf_s(strRefreshRate, 50, L"%d Hz", Mode.RefreshRate);
			strRefreshRate[49] = 0;

			// Is this already in the list ?
			if (!Combo_FindText(IDC_COMBO_REFRESH, strRefreshRate))
			{
				Combo_ItemAdd(IDC_COMBO_REFRESH, (LPVOID)Mode.RefreshRate, (wchar_t*)strRefreshRate);

				// If this is a setting we already have set, select it in the combo
				if (GetSettings()->displayMode.RefreshRate == Mode.RefreshRate)
					Combo_SelectItemData(IDC_COMBO_REFRESH, (LPVOID)Mode.RefreshRate);
			} // End if no text found
		} // End if matches
	} // Next Mode

	// If nothing is already selected, select the first item
	if (Combo_GetSelectedItemData(IDC_COMBO_REFRESH) == NULL && Combo_GetItemCount(IDC_COMBO_REFRESH) > 0)
		Combo_SelectItem(IDC_COMBO_REFRESH, 0);
}

void CD3DSettingsDlg::RefreshRateChanged()
{
	if (m_D3DSettings.windowed) return;
	//Update settings with new refresh rate
	GetSettings()->displayMode.RefreshRate = (ULONG)Combo_GetSelectedItemData(IDC_COMBO_REFRESH);
}

void CD3DSettingsDlg::BackBufferFormatChanged()
{
	ULONG i, j;

	CD3DEnumDevice * pDevice = (CD3DEnumDevice*)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);
	if (!pDevice) return;

	// Get selected formats
	D3DFORMAT adapterFormat = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_AFORMAT));
	D3DFORMAT bbFormat = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_BBFORMAT));

	// Loop through each option and update remaining list items.
	for (i = 0; i < pDevice->options.size(); i++)
	{
		CD3DEnumDeviceOptions * pDeviceOptions = pDevice->options[i];

		// Is this our selected mode ?
		if (pDeviceOptions->windowed == m_D3DSettings.windowed &&
			pDeviceOptions->adapterFormat == adapterFormat &&
			pDeviceOptions->backBufferFormat == bbFormat)
		{
			// Update settings with new options details
			GetSettings()->backBufferFormat = bbFormat;

			// Update depth format combo box
			Combo_Clear(IDC_COMBO_DSFORMAT);
			for (j = 0; j < pDeviceOptions->depthFormats.size(); j++)
			{
				D3DFORMAT Format = pDeviceOptions->depthFormats[j];
				Combo_ItemAdd(IDC_COMBO_DSFORMAT, (LPVOID)Format, D3DFormatToString(Format));

				// If this is a setting we already have set, select it in the combo
				if (Format == GetSettings()->depthStencilFormat)
					Combo_SelectItemData(IDC_COMBO_DSFORMAT, (LPVOID)Format);
			} // Next Format

			 // If nothing is already selected, select the first item
			if (Combo_GetSelectedItemData(IDC_COMBO_DSFORMAT) == NULL && Combo_GetItemCount(IDC_COMBO_DSFORMAT) > 0)
				Combo_SelectItem(IDC_COMBO_DSFORMAT, 0);

			// Update MultiSample type combo
			Combo_Clear(IDC_COMBO_MSTYPE);
			for (j = 0; j < pDeviceOptions->multiSampleTypes.size(); j++)
			{
				D3DMULTISAMPLE_TYPE Type = pDeviceOptions->multiSampleTypes[j];
				Combo_ItemAdd(IDC_COMBO_MSTYPE, (LPVOID)Type, MultisampleTypeToString(Type));

				// If this is a setting we already have set, select it in the combo
				if (Type == GetSettings()->multiSampleType)
					Combo_SelectItemData(IDC_COMBO_MSTYPE, (LPVOID)Type);

			} // Next Type

			// If nothing is already selected, select the first item
			if (Combo_GetSelectedItemData(IDC_COMBO_MSTYPE) == NULL && Combo_GetItemCount(IDC_COMBO_MSTYPE) > 0)
				Combo_SelectItem(IDC_COMBO_MSTYPE, 0);

			// Update VertexProcessing type combo
			Combo_Clear(IDC_COMBO_VPTYPE);
			for (j = 0; j < pDeviceOptions->vertexProcessingTypes.size(); j++)
			{
				VERTEXPROCESSING_TYPE Type = pDeviceOptions->vertexProcessingTypes[j];
				Combo_ItemAdd(IDC_COMBO_VPTYPE, (LPVOID)Type, VertexProcessingTypeToString(Type));

				// If this is a setting we already have set, select it in the combo
				if (Type == GetSettings()->vertexProcessingType)
					Combo_SelectItemData(IDC_COMBO_VPTYPE, (LPVOID)Type);
			} // Next Type

			// If nothing is already selected, select the first item
			if (Combo_GetSelectedItemData(IDC_COMBO_VPTYPE) == NULL && Combo_GetItemCount(IDC_COMBO_VPTYPE) > 0)
				Combo_SelectItem(IDC_COMBO_VPTYPE, 0);
			// Update PresentInterval combo
			Combo_Clear(IDC_COMBO_PINTERVAL);
			for (j = 0; j < pDeviceOptions->presentIntervals.size(); j++)
			{
				ULONG Interval = pDeviceOptions->presentIntervals[j];
				Combo_ItemAdd(IDC_COMBO_PINTERVAL, (LPVOID)Interval, PresentIntervalToString(Interval));

				// If this is a setting we already have set, select it in the combo
				if (Interval == GetSettings()->presentInterval)
					Combo_SelectItemData(IDC_COMBO_PINTERVAL, (LPVOID)Interval);
			} // Next Type

			// If nothing is already selected, select the first item
			if (Combo_GetSelectedItemData(IDC_COMBO_PINTERVAL) == NULL && Combo_GetItemCount(IDC_COMBO_PINTERVAL) > 0)
				Combo_SelectItem(IDC_COMBO_PINTERVAL, 0);

			break;
		} // End if Matches
	} // Next Option
}

void CD3DSettingsDlg::DepthStencilFormatChanged()
{
	D3DFORMAT Format = (D3DFORMAT)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_DSFORMAT));

	// Update settings with new format
	GetSettings()->depthStencilFormat = Format;
}

void CD3DSettingsDlg::MultisampleTypeChanged()
{
	ULONG               i;
	D3DMULTISAMPLE_TYPE MSType;
	MSType = (D3DMULTISAMPLE_TYPE)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_MSTYPE));

	// Update settings with new type
	GetSettings()->multiSampleType = MSType;

	// Find our selected device options
	CD3DEnumDevice * pDevice = (CD3DEnumDevice*)Combo_GetSelectedItemData(IDC_COMBO_DEVICE);
	if (!pDevice) return;

	CD3DEnumDeviceOptions * pDeviceOptions = NULL;
	for (i = 0; i < pDevice->options.size(); i++)
	{
		pDeviceOptions = pDevice->options[i];

		// Is this our selected mode ?
		if (pDeviceOptions->windowed == m_D3DSettings.windowed &&
			pDeviceOptions->adapterFormat == GetSettings()->displayMode.Format &&
			pDeviceOptions->backBufferFormat == GetSettings()->backBufferFormat) break;

		// Reset to NULL (used to determine if none was found )
		pDeviceOptions = NULL;

	} // Next Option Set

	  // Was any option set found ?
	if (!pDeviceOptions) return;

	// Find the maximum quality for this type
	ULONG  MaxQuality = 0;
	for (i = 0; i < pDeviceOptions->multiSampleTypes.size(); i++)
	{
		D3DMULTISAMPLE_TYPE Type = pDeviceOptions->multiSampleTypes[i];
		if (Type == MSType)
		{
			MaxQuality = pDeviceOptions->multiSampleQuality[i];
			break;

		} // End if Matches

	} // Next Type

	  // Update multi-sample quality combo
	Combo_Clear(IDC_COMBO_MSQUALITY);
	for (i = 0; i < MaxQuality; i++)
	{
		TCHAR strQuality[50];
		_sntprintf_s(strQuality, 50, L"%d", i);
		strQuality[49] = 0;

		// Add it to the combo
		Combo_ItemAdd(IDC_COMBO_MSQUALITY, (LPVOID)i, (wchar_t*)strQuality);

		// If this is a setting we already have set, select it in the combo
		if (i == GetSettings()->multisampleQuality)
			Combo_SelectItemData(IDC_COMBO_MSQUALITY, (LPVOID)i);

	} // Next Quality

	// If nothing is already selected, select the first item
	if (Combo_GetSelectedItemData(IDC_COMBO_MSQUALITY) == NULL && Combo_GetItemCount(IDC_COMBO_MSQUALITY) > 0)
		Combo_SelectItem(IDC_COMBO_MSQUALITY, 0);
}

void CD3DSettingsDlg::MultisampleQualityChanged()
{
	ULONG MSQuality = (ULONG)Combo_GetSelectedItemData(IDC_COMBO_MSQUALITY);

	// Update settings with new quality
	GetSettings()->multisampleQuality = MSQuality;
}

void CD3DSettingsDlg::VertexProcessingChanged()
{
	VERTEXPROCESSING_TYPE VPType;
	VPType = (VERTEXPROCESSING_TYPE)((ULONG)Combo_GetSelectedItemData(IDC_COMBO_VPTYPE));

	// Update settings with new type
	GetSettings()->vertexProcessingType = VPType;
}

void CD3DSettingsDlg::PresentIntervalChanged()
{
	ULONG Interval = (ULONG)Combo_GetSelectedItemData(IDC_COMBO_PINTERVAL);

	// Update settings with new interval
	GetSettings()->presentInterval = Interval;
}

void CD3DSettingsDlg::Combo_ItemAdd(ULONG ComboID, LPVOID pData, LPCTSTR pStrDesc)
{
	// Get the control handle
	HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);

	// Add a new 'String' item to the combo box
	DWORD dwItem = ComboBox_AddString(hwndCtrl, pStrDesc);

	// Set it's additional data
	ComboBox_SetItemData(hwndCtrl, dwItem, pData);
}

LPVOID CD3DSettingsDlg::Combo_GetSelectedItemData(ULONG ComboID)
{
	// Get the control handle
	HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);

	// Retrieve the currently selected item
	int iItem = ComboBox_GetCurSel(hwndCtrl);

	// Validate
	if (iItem < 0) return NULL;

	// Return the data pointer
	return (LPVOID)ComboBox_GetItemData(hwndCtrl, iItem);
}

ULONG CD3DSettingsDlg::Combo_GetItemCount(ULONG ComboID)
{
	// Get the control handle
	HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);

	// Return the number of items
	return ComboBox_GetCount(hwndCtrl);
}

void CD3DSettingsDlg::Combo_SelectItem(ULONG ComboID, ULONG Index)
{
	// Get the control handle
	HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);

	// Set the current selection
	ComboBox_SetCurSel(hwndCtrl, Index);

	// Trigger a combo box selection change message
	PostMessage(m_hWndDlg, WM_COMMAND, MAKEWPARAM(ComboID, CBN_SELCHANGE), (LPARAM)hwndCtrl);
}

void CD3DSettingsDlg::Combo_SelectItemData(ULONG ComboID, LPVOID pData)
{
	// Get the control handle
	HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);

	// Loop through each item.
	ULONG Count = ComboBox_GetCount(hwndCtrl);
	for (ULONG i = 0; i < Count; i++)
	{
		// Does it match ?
		if ((LPVOID)ComboBox_GetItemData(hwndCtrl, i) == pData)
		{
			// Set the current selection
			ComboBox_SetCurSel(hwndCtrl, i);

			// Trigger a combo box selection change message
			PostMessage(m_hWndDlg, WM_COMMAND, MAKEWPARAM(ComboID, CBN_SELCHANGE), (LPARAM)hwndCtrl);

			// Bail
			return;
		} // End if Match
	} // Next Item
}

void CD3DSettingsDlg::Combo_Clear(ULONG ComboID)
{
	// Get the control handle
	HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);

	// Reset the combo
	ComboBox_ResetContent(hwndCtrl);
}

bool CD3DSettingsDlg::Combo_FindText(ULONG ComboID, LPCTSTR pStrText)
{
	TCHAR strItem[255];

	// Get the control handle
	HWND hwndCtrl = GetDlgItem(m_hWndDlg, ComboID);

	// Loop through each item.
	ULONG Count = ComboBox_GetCount(hwndCtrl);
	for (ULONG i = 0; i < Count; i++)
	{
		// Skip if greater than max size
		if (ComboBox_GetLBTextLen(hwndCtrl, i) >= 255) continue;

		// Retrieve the text
		ComboBox_GetLBText(hwndCtrl, i, strItem);

		// Does it match ?
		if (lstrcmp(strItem, pStrText) == 0) return true;

	} // Next Item

	  // No match found
	return false;
}

