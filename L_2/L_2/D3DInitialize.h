#pragma once
#include "stdafx.h"
#include <d3dx9.h>
#include <vector>

//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------
class CD3DEnumDeviceOptions;
class CD3DEnumDevice;
class CD3DEnumAdapter;

//-----------------------------------------------------------------------------
// Name: VertexProcessingType (Enum)
// Desc: Enumeration of all possible D3D vertex processing types.
//-----------------------------------------------------------------------------
enum VERTEXPROCESSING_TYPE
{
	SOFTWARE_VP = 1, //Software Vertex Processing
	MIXED_VP = 2, //Mixed vertex processing
	HARDWARE_VP = 3, //Hardware vertex processing
	PURE_HARDWARE_VP = 4//Pure hardware (no querying)
};

//-----------------------------------------------------------------------------
// STL Vector Typedefs for Easy Access
//-----------------------------------------------------------------------------
typedef std::vector<D3DMULTISAMPLE_TYPE> VectorMSType;
typedef std::vector<D3DFORMAT> VectorFormat;
typedef std::vector<ULONG> VectorULONG;
typedef std::vector<VERTEXPROCESSING_TYPE>  VectorVPType;
typedef std::vector<D3DDISPLAYMODE>         VectorDisplayMode;
typedef std::vector<CD3DEnumDeviceOptions*> VectorDeviceOptions;
typedef std::vector<CD3DEnumDevice*>        VectorDevice;
typedef std::vector<CD3DEnumAdapter*>       VectorAdapter;

class CD3DEnumAdapter
{
public:
	~CD3DEnumAdapter();

	ULONG ordinal;
	D3DADAPTER_IDENTIFIER9 identifier;//holds information about the hardware (name, driver, manufacturer...)
	VectorDisplayMode modes;//Width, height, pixel format and refresh rate
	VectorDevice devices;//Store all device types available (HAL or REF)
};

class CD3DEnumDevice
{
public:
	~CD3DEnumDevice();

	D3DDEVTYPE DeviceType;
	D3DCAPS9 caps;
	VectorDeviceOptions options;
};

class CD3DEnumDeviceOptions
{
public:
	~CD3DEnumDeviceOptions();

	ULONG adapterOrdinal;
	D3DDEVTYPE deviceType;
	D3DCAPS9 caps;
	D3DFORMAT adapterFormat;
	D3DFORMAT backBufferFormat;
	bool windowed;
	VectorMSType multiSampleTypes;
	VectorULONG multiSampleQuality;
	VectorFormat depthFormats;
	VectorVPType vertexProcessingTypes;
	VectorULONG presentIntervals;
};

class CD3DSettings
{
public:
	struct Settings
	{
		ULONG adapterOrdinal;
		D3DDISPLAYMODE displayMode;
		D3DDEVTYPE deviceType;
		D3DFORMAT backBufferFormat;
		D3DFORMAT depthStencilFormat;
		D3DMULTISAMPLE_TYPE multiSampleType;
		ULONG multisampleQuality;
		VERTEXPROCESSING_TYPE vertexProcessingType;
		ULONG presentInterval;
	};

	bool windowed;
	Settings* GetSettings() { return (windowed) ? &windowed_Settings : &fullScreen_Settings; }

private:
	Settings windowed_Settings;
	Settings fullScreen_Settings;
};

class CD3DInitialize
{
public:
	CD3DInitialize();
	virtual ~CD3DInitialize();

	//--------------------------------------------------------
	//Public Functions for this class
	//--------------------------------------------------------
	HRESULT Enumerate(LPDIRECT3D9 pD3D);
	//Creates the device object and (optionally) a window (if hWnd is NULL, the next 4 parameters will be used to create a window)
	HRESULT CreateDisplay(CD3DSettings& D3DSettings, ULONG flags = 0, HWND hWnd = nullptr, WNDPROC pWndProc = nullptr, LPCTSTR title = nullptr, 
		ULONG width = CW_USEDEFAULT, ULONG height = CW_USEDEFAULT, LPVOID lParam = nullptr);
	HRESULT ResetDisplay(LPDIRECT3DDEVICE9 pD3DDevice, CD3DSettings& D3DSettings, HWND hWnd = nullptr);
	bool FindBestWindowedMode(CD3DSettings& D3DSettings, bool bRequireHAL = false, bool bRequireREF = false);
	bool FindBestFullscreenMode(CD3DSettings& D3DSettings, D3DDISPLAYMODE * pMatchMode = nullptr, bool bRequireHAL = false, bool bRequireREF = false);
	D3DPRESENT_PARAMETERS BuildPresentParameters(CD3DSettings& D3DSettings, ULONG flags = 0);
	ULONG GetAdapterCount() const { return m_vpAdapters.size(); }
	const CD3DEnumAdapter *GetAdapter(ULONG index) { return (index > GetAdapterCount()) ? nullptr : m_vpAdapters[index]; }
	HWND GetHWND() { return m_hWnd; }
	const LPDIRECT3D9 GetDirect3D() { return m_pD3D; }
	LPDIRECT3DDEVICE9 GetDirect3DDevice();

private:

	//--------------------------------------------------------
	// Private Functions for this class
	//--------------------------------------------------------
	HRESULT EnumerateAdapters();
	HRESULT EnumerateDisplayModes(CD3DEnumAdapter* pAdapter);
	HRESULT EnumerateDevices(CD3DEnumAdapter* pAdapter);
	HRESULT EnumerateDeviceOptions(CD3DEnumDevice* pDevice, CD3DEnumAdapter* pAdapter);
	HRESULT EnumerateDepthStencilFormats(CD3DEnumDeviceOptions* pDeviceOptions);
	HRESULT EnumerateMultiSampleTypes(CD3DEnumDeviceOptions* pDeviceOptions);
	HRESULT EnumerateVertexProcessingTypes(CD3DEnumDeviceOptions* pDeviceOptions);
	HRESULT EnumeratePresentIntervals(CD3DEnumDeviceOptions* pDeviceOptions);

	//Private Virtual Functions (can be used by derived class to reject some devices and adapters that do not meet the minimun requirements
	virtual bool        ValidateDisplayMode(const D3DDISPLAYMODE& Mode) { return true; }
	virtual bool        ValidateDevice(const D3DDEVTYPE& Type, const D3DCAPS9& Caps) { return true; }
	virtual bool        ValidateDeviceOptions(const D3DFORMAT& BackBufferFormat, bool IsWindowed) { return true; }
	virtual bool        ValidateDepthStencilFormat(const D3DFORMAT& DepthStencilFormat) { return true; }
	virtual bool        ValidateMultiSampleType(const D3DMULTISAMPLE_TYPE& Type) { return true; }
	virtual bool        ValidateVertexProcessingType(const VERTEXPROCESSING_TYPE& Type) { return true; }
	virtual bool        ValidatePresentInterval(const ULONG& Interval) { return true; }

	//--------------------------------------------------------
	// Private Variables for this class
	//--------------------------------------------------------
	LPDIRECT3D9 m_pD3D;//Primary Direct3D object
	LPDIRECT3DDEVICE9 m_pD3DDevice;//Create Direct3D device
	HWND m_hWnd; //Create window handl
	VectorAdapter m_vpAdapters; //Enumerated adapters
};

