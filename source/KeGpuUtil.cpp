#include <Ke.h>
#include <KeGpuUtil.h>
#include <iostream>


/*
 * NVIDIA GPU utility functions
 */
#ifdef KE_ENABLE_NVAPI

#ifdef _WIN32
#include <nvapi.h>
#endif


#if defined(_M_X64) || defined(__amd64__)
#define NVAPI_DLL "nvapi64.dll"
#else
#define NVAPI_DLL "nvapi.dll"
#endif


// magic numbers, do not change them
#define NVAPI_MAX_PHYSICAL_GPUS   64
#define NVAPI_MAX_USAGES_PER_GPU  34
     
// function pointer types
typedef int *(*NvAPI_QueryInterface_t)(unsigned int offset);
typedef int (*NvAPI_Initialize_t)();
typedef int (*NvAPI_EnumPhysicalGPUs_t)(int **handles, int *count);
typedef int (*NvAPI_GPU_GetUsages_t)(int *handle, unsigned int *usages);

// nvapi.dll internal function pointers
NvAPI_QueryInterface_t      _NvAPI_QueryInterface     = NULL;
NvAPI_Initialize_t          _NvAPI_Initialize         = NULL;
NvAPI_EnumPhysicalGPUs_t    _NvAPI_EnumPhysicalGPUs   = NULL;
NvAPI_GPU_GetUsages_t       _NvAPI_GPU_GetUsages      = NULL;



/*
 * Name: NvGpuDetected
 * Desc: Returns true if an NVIDIA Gpu has been detected on this system.
 * NOTE: This function depends on whether a valid NVIDIA driver is installed
 *       on the target machine.  Since the Surface Hub does not include an
 *		 nvapi[64].dll in it's strippified driver, we need to load it directly 
 *		 in order for the required APIs to work.
 */
BOOL NvGpuDetected()
{
#ifdef _WIN32
    HMODULE hmod = LoadLibraryA( NVAPI_DLL );

	if( hmod == NULL )
    {
        DISPDBG( KE_ERROR, "Couldn't find " << NVAPI_DLL );
        return FALSE;
    }

	// nvapi_QueryInterface is a function used to retrieve other internal functions in nvapi.dll
    _NvAPI_QueryInterface = (NvAPI_QueryInterface_t) GetProcAddress( hmod, "nvapi_QueryInterface" );
     
    // some useful internal functions that aren't exported by nvapi.dll
    _NvAPI_Initialize = (NvAPI_Initialize_t) (*_NvAPI_QueryInterface)(0x0150E828);
    _NvAPI_EnumPhysicalGPUs = (NvAPI_EnumPhysicalGPUs_t) (*_NvAPI_QueryInterface)(0xE5AC921F);
    _NvAPI_GPU_GetUsages = (NvAPI_GPU_GetUsages_t) (*_NvAPI_QueryInterface)(0x189A1FDF);
     
    if( _NvAPI_Initialize == NULL || _NvAPI_EnumPhysicalGPUs == NULL ||
        _NvAPI_QueryInterface == NULL || _NvAPI_GPU_GetUsages == NULL )
    {
        DISPDBG( KE_ERROR, "Couldn't hook functions in " << NVAPI_DLL );
        return FALSE;
    }
     
    // initialize NvAPI library, call it once before calling any other NvAPI functions
    if( (*_NvAPI_Initialize)() != 0 )
	{
		DISPDBG( KE_ERROR, "Could not initialize NVAPI!" );
		return FALSE;
	}

	return TRUE;
#else
	return FALSE;
#endif
}

/*
 * Name: NvGetGpuLoad
 * Desc: Returns the NVIDIA Gpu's current load percentage.
 */
int  NvGetGpuLoad()
{
#ifdef _WIN32
	int          gpuCount = 0;
    int         *gpuHandles[NVAPI_MAX_PHYSICAL_GPUS] = { NULL };
    unsigned int gpuUsages[NVAPI_MAX_USAGES_PER_GPU] = { 0 };
     
    // gpuUsages[0] must be this value, otherwise NvAPI_GPU_GetUsages won't work
    gpuUsages[0] = (NVAPI_MAX_USAGES_PER_GPU * 4) | 0x10000;
     
    (*_NvAPI_EnumPhysicalGPUs)( gpuHandles, &gpuCount );
     
    (*_NvAPI_GPU_GetUsages)( gpuHandles[0], gpuUsages );
    int usage = gpuUsages[3];

	return usage;
#else
	return 0;
#endif
}

/*
 * Name: NvGetGpuTemperature
 * Desc: Returns the current temperature of an NVIDIA Gpu.
 */
int  NvGetGpuTemperature()
{
#ifdef _WIN32
	// Array of physical GPU handle
	NvPhysicalGpuHandle nvGPUHandles[ NVAPI_MAX_PHYSICAL_GPUS];
	// Thermal settings
	NV_GPU_THERMAL_SETTINGS temperature;
	// Error string
	NvAPI_ShortString string;

	// ---- Retrive the handle for the GPU ----

	NvU32 gpuCount = 0;
	NvAPI_Status status = NvAPI_EnumPhysicalGPUs( nvGPUHandles, &gpuCount );
	if( status != NVAPI_OK ) // !TODO: cache the table for drivers >= 105.00
	{
		NvAPI_GetErrorMessage( status, string );
		DISPDBG( KE_ERROR, "NvAPI_EnumPhysicalGPUs(): " << string );
		return -1;
	}

	// Ensure the index is correct
//	if( dwGPUIndex > gpuCount )
//		return -1;

	// Retrive the temperature
	ZeroMemory( &temperature, sizeof( NV_GPU_THERMAL_SETTINGS ) );
	temperature.version = NV_GPU_THERMAL_SETTINGS_VER;
	status = NvAPI_GPU_GetThermalSettings( nvGPUHandles[0], NVAPI_THERMAL_TARGET_ALL, &temperature );
	if( status != NVAPI_OK )
	{
		NvAPI_GetErrorMessage( status, string );
		DISPDBG( KE_ERROR, "\n\tNvAPI_GPU_GetThermalSettings(): " << string );
		return -1;
	}

	if( temperature.count == 0 )
	{
		DISPDBG( KE_WARNING, "temperature.count == 0" );
		return -1;
	}

	return temperature.sensor[0].currentTemp;
#else
	return 0;
#endif
}

#endif // NVAPI



/*
 * D3DKMT GPU utilities (Intel)
 */
#if defined(KE_ENABLE_D3DKMT) && defined(_WIN32)

#include <d3dkmt.h>
#include <d3d11_2.h>
#include <comip.h>
#include <comdef.h>
#include <string.h>
#include <wbemidl.h>
//#include <ntdef.h>


#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

#define UpdateDelta( DltMgr, NewValue ) 	\
	((DltMgr)->Delta = (NewValue) - (DltMgr)->Value, \
    (DltMgr)->Value = (NewValue), (DltMgr)->Delta)


struct UINT64_DELTA
{
	ULONG64 Value;
	ULONG64 Delta;
};

typedef _com_ptr_t<_com_IIID<ID3D11Device, &IID_ID3D11Device>> CD3D11Device;
typedef _com_ptr_t<_com_IIID<IDXGIDevice, &IID_IDXGIDevice>> CDXGIDevice;
typedef _com_ptr_t<_com_IIID<IDXGIAdapter, &IID_IDXGIAdapter>> CDXGIAdapter;
typedef _com_ptr_t<_com_IIID<IDXGIAdapter1, &IID_IDXGIAdapter1>> CDXGIAdapter1;
typedef _com_ptr_t<_com_IIID<IDXGIAdapter2, &IID_IDXGIAdapter2>> CDXGIAdapter2;
typedef _com_ptr_t<_com_IIID<ID3D11DeviceContext, &IID_ID3D11DeviceContext>> CD3D11DeviceContext;
typedef _com_ptr_t<_com_IIID<ID3D11Device2, &IID_ID3D11Device2>> CD3D11Device2;
typedef _com_ptr_t<_com_IIID<IDXGIAdapter2, &IID_IDXGIAdapter2>> CDXGIAdapter2;
typedef _com_ptr_t<_com_IIID<IDXGIFactory, &IID_IDXGIFactory>> CDXGIFactory;
typedef _com_ptr_t<_com_IIID<IDXGIFactory1, &IID_IDXGIFactory1>> CDXGIFactory1;
typedef _com_ptr_t<_com_IIID<IDXGIFactory2, &IID_IDXGIFactory2>> CDXGIFactory2;


PFND3DKMT_QUERYSTATISTICS	pfnD3DKMTQueryStatistics = nullptr;
HMODULE						hGdi = nullptr;
DXGI_ADAPTER_DESC2			DxgiDesc;
UINT						AdapterNumber = 0;
std::vector<float>			GpuUsage;
UINT64_DELTA				ClockTotalRunningTimeDelta = {0};
LARGE_INTEGER				ClockTotalRunningTimeFrequency = {0};
UINT64_DELTA				GpuTotalRunningTimeDelta = {0};
UINT64_DELTA				GpuSystemRunningTimeDelta = {0};
UINT64_DELTA				GpuNodesTotalRunningTimeDelta[16];


/*
 * Name: KmtGetAdapter
 * Desc: Returns the adapter description for the specified display adapter index.
 *		 Relies on DXGI to get the information needed.
 * TODO: Enumerate GPUs without the use of DXGI.
 */
HRESULT KmtGetAdapter( UINT Index, DXGI_ADAPTER_DESC2* Desc )
{
	CDXGIFactory2 DXGIFactory;
	CDXGIAdapter1 Adapter1;
	CDXGIAdapter2 Adapter2;

	HRESULT hr = CreateDXGIFactory2( 0, __uuidof( CDXGIFactory2 ), (void**) &DXGIFactory );
	if( FAILED( hr ) )
		return hr;

	hr = DXGIFactory->EnumAdapters1( Index, &Adapter1 );
	if( FAILED( hr ) )
		return hr;

	hr = Adapter1->QueryInterface( __uuidof( Adapter1 ), (void**) &Adapter2 );

	return Adapter2->GetDesc2( Desc );
}


/*
 * Name: KmtGetNodeCount
 * Desc: Returns the number of GPU nodes associated with this GPU.
 */
UINT KmtGetNodeCount()
{
	D3DKMT_QUERYSTATISTICS QueryStatistics;
	memset( &QueryStatistics, 0, sizeof( D3DKMT_QUERYSTATISTICS ) );
	QueryStatistics.Type = D3DKMT_QUERYSTATISTICS_ADAPTER;
	QueryStatistics.AdapterLuid = DxgiDesc.AdapterLuid;

	NTSTATUS status = pfnD3DKMTQueryStatistics( &QueryStatistics );
	if( !NT_SUCCESS( status ) )
		return 0;

	return QueryStatistics.QueryResult.AdapterInformation.NodeCount;
}


/*
 * Name: KmtDetectGpu
 * Desc: Uses Direct3D KMT to detect an Intel GPU for Whitney devices.  If the Intel
 *		 GPU hardware is not found, then this function returns a failure.  Once the
 *		 expected GPU is found, we open up GDI32.dll and get a function pointer to the
 *		 function required to monitor GPU specific stats.
 */
BOOL KmtDetectGpu()
{
	UINT i = 0;

	/* Search for an Intel GPU.  If we don't find one, exit. */

	do {
		if( SUCCEEDED( KmtGetAdapter( i, &DxgiDesc ) ) )
		{
			if( wcsstr( DxgiDesc.Description, L"Intel(R)" ) != NULL )
				break;
		}
		else 
		{
			DISPDBG( KE_ERROR, "Intel GPU not found!" );
			return FALSE;
		}

		i++;
	} while(true);

	/* Load the gdi32.dll directly, and get a function pointer to the necessary function. */

	hGdi = ::LoadLibrary( TEXT( "gdi32.dll" ) );
	if( !hGdi )
	{
		DISPDBG( KE_ERROR, "Unable to open gdi32.dll!" );
		return FALSE;
	}

	pfnD3DKMTQueryStatistics = (PFND3DKMT_QUERYSTATISTICS) GetProcAddress( hGdi, "D3DKMTQueryStatistics" );
	if( !pfnD3DKMTQueryStatistics )
	{
		DISPDBG( KE_ERROR, "Unable to locate D3DKMTQueryStatistics()!" );
		return FALSE;
	}

	FreeLibrary( hGdi );

	return TRUE;
}

/*
 * Name: KmtGetGpuUsage
 * Desc: Returns the GPU usage in the form of a percentage.  
 */
int  KmtGetGpuUsage()
{
	UINT64 SharedBytesUsed = 0, DedicatedBytesUsed = 0, CommittedBytesUsed = 0;
	D3DKMT_QUERYSTATISTICS QueryStatistics;
	ULONG i;
	ULONG64 TotalRunningTime = 0, SystemRunningTime = 0;
	UINT NodeCount = KmtGetNodeCount();
	DOUBLE ElapsedTime;

	/* Query the statistics of each node and determine the level of running time for each one. */

	for( i = 0; i < NodeCount; i++ )
	{
		memset( &QueryStatistics, 0, sizeof( D3DKMT_QUERYSTATISTICS ) );
		QueryStatistics.Type = D3DKMT_QUERYSTATISTICS_NODE;
		QueryStatistics.AdapterLuid = DxgiDesc.AdapterLuid;
		QueryStatistics.QueryNode.NodeId = i;

		NTSTATUS status = pfnD3DKMTQueryStatistics( &QueryStatistics );
		if( NT_SUCCESS( status ) )
		{
			UpdateDelta( &GpuNodesTotalRunningTimeDelta[i], QueryStatistics.QueryResult.NodeInformation.GlobalInformation.RunningTime.QuadPart );
			TotalRunningTime += QueryStatistics.QueryResult.NodeInformation.GlobalInformation.RunningTime.QuadPart;
			SystemRunningTime += QueryStatistics.QueryResult.NodeInformation.SystemInformation.RunningTime.QuadPart;
		}
	}

	/* Update timing */

	LARGE_INTEGER PerformanceCounter;

	QueryPerformanceCounter( &PerformanceCounter );
	QueryPerformanceFrequency( &ClockTotalRunningTimeFrequency );
	UpdateDelta( &ClockTotalRunningTimeDelta, PerformanceCounter.QuadPart );
	UpdateDelta( &GpuTotalRunningTimeDelta, TotalRunningTime );
	UpdateDelta( &GpuSystemRunningTimeDelta, SystemRunningTime );

	ElapsedTime = (DOUBLE) ClockTotalRunningTimeDelta.Delta * 10000000 / ClockTotalRunningTimeFrequency.QuadPart;

	float GpuUsage = 0;
	int PositiveNodes = 0;

	/* At the moment, we are only returning the GPU usage for the first node.  I noticed that 
	   with Intel GPUs, the first node is the one that is used primarily, and the other nodes
	   don't seem to get used much, if at all.  In the future, we should probably take them all
	   into account and average the results together, but for now, the first node tends to give
	   rather consistent results so far. */

	//for( i = 0; i < NodeCount; i++ )
	//{
		if( ElapsedTime != 0 )
		{
			GpuUsage = (float) ( GpuTotalRunningTimeDelta.Delta / ElapsedTime );
		}
	//}

	if( GpuUsage > 1.0f )
		GpuUsage = 1.0f;

	return int( GpuUsage * 100.0f );
}


/*
 * Name: KmtGetGpuTemperature
 * Desc: Returns the temperature of the GPU.
 * NOTE: Since the intel GPU is integrated with the same die as the CPU itself, this will
 *		 probably be the same as CPU temperature itself.
 */
int  KmtGetGpuTemperature()
{
#if 0	/* TODO: Find out why this implementation does not work */
	ULONG Temperature = -1;
    
    HRESULT ci = CoInitialize(NULL); // needs comdef.h
    HRESULT hr = CoInitializeSecurity( NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL );

    if( SUCCEEDED( hr ) )
    {
        IWbemLocator *pLocator; // needs Wbemidl.h & Wbemuuid.lib
        hr = CoCreateInstance( CLSID_WbemAdministrativeLocator, NULL, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*) &pLocator );
        if( SUCCEEDED( hr ) )
        {
            IWbemServices *pServices;
            BSTR ns = SysAllocString( L"root\\WMI" );
            hr = pLocator->ConnectServer( ns, NULL, NULL, NULL, 0, NULL, NULL, &pServices );
            pLocator->Release();
            SysFreeString(ns);
            if( SUCCEEDED( hr ) )
            {
                BSTR query = SysAllocString( L"SELECT * FROM MSAcpi_ThermalZoneTemperature" );
                BSTR wql = SysAllocString( L"WQL" );
                IEnumWbemClassObject *pEnum;
                hr = pServices->ExecQuery( wql, query, WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY, NULL, &pEnum );
                SysFreeString(wql);
                SysFreeString(query);
                pServices->Release();
                if( SUCCEEDED( hr ) )
                {
                    IWbemClassObject *pObject;
                    ULONG returned;
                    hr = pEnum->Next( WBEM_INFINITE, 1, &pObject, &returned );
                    pEnum->Release();
                    if( SUCCEEDED( hr ) )
                    {
                        BSTR temp = SysAllocString( L"CurrentTemperature" );
                        VARIANT v;
                        VariantInit(&v);
                        hr = pObject->Get( temp, 0, &v, NULL, NULL );
                        pObject->Release();
                        SysFreeString(temp);
                        if( SUCCEEDED( hr ) )
                        {
                            Temperature = V_I4(&v);
                        }
                        VariantClear(&v);
                    }
                }
            }
            if( ci == S_OK )
            {
                CoUninitialize();
            }
        }
    }

	return Temperature;
#else
	return 0;
#endif
}

#endif // D3DKMT && _WIN32


/*
 * Name: NvGpuDetected
 * Desc: Returns true if supported Gpu has been detected on this system.
 * NOTE: This function is heavily OS dependent and is not guaranteed to
 *		 have support for your given Gpu.  
 */
BOOL KeGpuDetected()
{
#ifdef KE_ENABLE_NVAPI
	return NvGpuDetected();
#endif

#ifdef KE_ENABLE_D3DKMT
	return KmtDetectGpu();
#endif

	return FALSE;
}

/*
 * Name: NvGetGpuLoad
 * Desc: Returns the Gpu's current load percentage if available.
 */
int  KeGetGpuLoad()
{
#ifdef KE_ENABLE_NVAPI
	return NvGetGpuLoad();
#endif

#ifdef KE_ENABLE_D3DKMT
	return KmtGetGpuUsage();
#endif

	return 0;
}

/*
 * Name: NvGetGpuTemperature
 * Desc: Returns the current temperature of the Gpu if available.
 */
int  KeGetGpuTemperature()
{
#ifdef KE_ENABLE_NVAPI
	return NvGetGpuTemperature();
#endif

#ifdef KE_ENABLE_D3DKMT
	return KmtGetGpuTemperature();
#endif

	return 0;
}