#pragma once
#include "D3DInitialize.h"
#include <tchar.h>
#include <windows.h>
#include <windowsx.h>

class CD3DSettingsDlg
{
public:
	CD3DSettingsDlg();
	virtual ~CD3DSettingsDlg();

	int ShowDialog(CD3DInitialize* pInitialize, CD3DSettings* pSettings = nullptr, HWND hWndParent = nullptr);
	CD3DSettings GetD3DSettings() const { return m_D3DSettings; }

private:
	//-------------------------------------------------------------------------
	// Private Functions for This Class
	//-------------------------------------------------------------------------
	BOOL			SettingsDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
	void            AdapterChanged();
	void            DeviceChanged();
	void            WindowedFullscreenChanged();
	void            AdapterFormatChanged();
	void            ResolutionChanged();
	void            RefreshRateChanged();
	void            BackBufferFormatChanged();
	void            DepthStencilFormatChanged();
	void            MultisampleTypeChanged();
	void            MultisampleQualityChanged();
	void            VertexProcessingChanged();
	void            PresentIntervalChanged();

	void            Combo_ItemAdd(ULONG ComboID, LPVOID pData, LPCTSTR pStrDesc);
	LPVOID          Combo_GetSelectedItemData(ULONG ComboID);
	ULONG           Combo_GetItemCount(ULONG ComboID);
	void            Combo_SelectItem(ULONG ComboID, ULONG Index);
	void            Combo_SelectItemData(ULONG ComboID, LPVOID pData);
	void            Combo_Clear(ULONG ComboID);
	bool            Combo_FindText(ULONG ComboID, LPCTSTR pStrText);

	CD3DSettings::Settings* GetSettings() { return m_D3DSettings.GetSettings(); }

	//-------------------------------------------------------------------------
	// Private Static Functions For This Class
	//-------------------------------------------------------------------------
	static BOOL CALLBACK StaticDlgProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	//-------------------------------------------------------------------------
	// Private Variables For This Class
	//-------------------------------------------------------------------------
	CD3DInitialize *m_pInitialize;			// The class which stores enum objects
	HWND            m_hWndDlg;              // The main dialog window handle 
	CD3DSettings    m_D3DSettings;          // The settings we are currently using
};

