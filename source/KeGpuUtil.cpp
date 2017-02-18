#include <Ke.h>
#include <KeGpuUtil.h>
#include <iostream>


/*
 * NVIDIA GPU utility functions
 */

#ifdef _WIN32
#include <nvapi.h>
#endif


#ifdef _UWP
#if defined(_M_X64) || defined(__amd64__)
#define NVAPI_DLL L"nvapi64.dll"
#else
#define NVAPI_DLL L"nvapi.dll"
#endif
#else
#if defined(_M_X64) || defined(__amd64__)
#define NVAPI_DLL "nvapi64.dll"
#else
#define NVAPI_DLL "nvapi.dll"
#endif
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
 * NVIDIA GPU monitor class
 */
struct IKeGpuMonitorNV : public IKeGpuMonitor
{
    /*
     * Name: IKeGpuMonitorNV::Destroy()
     * Desc: If a handle to the nvapi[64].dll exists, close it.
     */
    KEMETHOD Destroy()
    {
#ifdef _WIN32
        /* Close the DLL handle, if it is open */
        if( hmod )
            CloseHandle( hmod );
        
        /* TODO: Does NVAPI need to be 'uninitialized'? */
#endif
    }
    
    /*
     * Name: IKeGpuMonitorNV::Detect
     * Desc: Returns true if an NVIDIA Gpu has been detected on this system.
     * NOTE: This function depends on whether a valid NVIDIA driver is installed
     *       on the target machine.  Since the Surface Hub does not include an
     *		 nvapi[64].dll in it's strippified driver, we need to load it directly
     *		 in order for the required APIs to work.
     */
    _KEMETHOD(bool) Detect()
    {
#ifdef _WIN32
#ifndef _UWP
        HMODULE hmod = LoadLibraryA( NVAPI_DLL );
#else
		HMODULE hmod = LoadPackagedLibrary( NVAPI_DLL, 0 );
#endif
        if( hmod == NULL )
        {
            DISPDBG( KE_ERROR, "Couldn't find " << NVAPI_DLL );
            return false;
        }
        
        // nvapi_QueryInterface is a function used to retrieve other internal functions in nvapi.dll
        _NvAPI_QueryInterface = (NvAPI_QueryInterface_t) GetProcAddress( hmod, "nvapi_QueryInterface" );
        
        // some useful internal functions that aren't exported by nvapi.dll
        _NvAPI_Initialize = (NvAPI_Initialize_t) (*_NvAPI_QueryInterface)(0x0150E828);
        _NvAPI_EnumPhysicalGPUs = (NvAPI_EnumPhysicalGPUs_t) (*_NvAPI_QueryInterface)(0xE5AC921F);
        _NvAPI_GPU_GetUsages = (NvAPI_GPU_GetUsages_t) (*_NvAPI_QueryInterface)(0x189A1FDF);
        
		FreeLibrary( hmod );

        if( _NvAPI_Initialize == NULL || _NvAPI_EnumPhysicalGPUs == NULL ||
           _NvAPI_QueryInterface == NULL || _NvAPI_GPU_GetUsages == NULL )
        {
            DISPDBG( KE_ERROR, "Couldn't hook functions in " << NVAPI_DLL );
            return false;
        }
        
        // initialize NvAPI library, call it once before calling any other NvAPI functions
        if( (*_NvAPI_Initialize)() != 0 )
        {
            DISPDBG( KE_ERROR, "Could not initialize NVAPI!" );
            return false;
        }
        
        return true;
#else
        return false;
#endif
    }
    
    /*
     * Name: IKeGpuMonitorNV::GetDescription
     * Desc: Returns information that describes this GPU, such as the
     *       vendor and device IDs, total amount of VRAM, etc.
     */
    KEMETHOD GetDescription( KeGpuDescriptor* desc )
    {
        /* Sanity check */
        if( !desc )
        {
            DISPDBG( KE_ERROR, "Invalid parameter!" );
            return;
        }
        
        /* TODO */
    }
    
    /*
     * Name: IKeGpuMonitorNV::GetStatus
     * Desc: Returns the GPU's various stats, such as GPU load percentage,
     *       current temperature, available video memory, as well as the
     *       current highest/lowest values of each attribute.
     * NOTE: None of the above stats are guaranteed to be available. It 
     *       often varies from one GPU to the next.  NV Quadro has been
     *       known to have issues with temperature reporting (with this
     *       code below).
     */
    KEMETHOD GetStatus( KeGpuStatus* status )
    {
        /*
         * GPU Usage
         */
#ifdef _WIN32
        int          gpuCount = 0;
        int         *gpuHandles[NVAPI_MAX_PHYSICAL_GPUS] = { NULL };
        unsigned int gpuUsages[NVAPI_MAX_USAGES_PER_GPU] = { 0 };
        
        // gpuUsages[0] must be this value, otherwise NvAPI_GPU_GetUsages won't work
        gpuUsages[0] = (NVAPI_MAX_USAGES_PER_GPU * 4) | 0x10000;
        
        (*_NvAPI_EnumPhysicalGPUs)( gpuHandles, &gpuCount );
        
        (*_NvAPI_GPU_GetUsages)( gpuHandles[0], gpuUsages );
        status->usage = gpuUsages[3];
#endif
        
        /*
         * GPU temperature
         */
#ifdef _WIN32
        // Array of physical GPU handle
        NvPhysicalGpuHandle nvGPUHandles[ NVAPI_MAX_PHYSICAL_GPUS];
        // Thermal settings
        NV_GPU_THERMAL_SETTINGS temperature;
        // Error string
        NvAPI_ShortString string;
        
        // ---- Retrive the handle for the GPU ----
        NvU32 GpuCount;
        NvAPI_Status nvstatus = NvAPI_EnumPhysicalGPUs( nvGPUHandles, &GpuCount );
        if( nvstatus != NVAPI_OK ) // !TODO: cache the table for drivers >= 105.00
        {
            NvAPI_GetErrorMessage( nvstatus, string );
            DISPDBG( KE_ERROR, "NvAPI_EnumPhysicalGPUs(): " << string );
        }
        
        // Ensure the index is correct
        //	if( dwGPUIndex > gpuCount )
        //		return -1;
        
        // Retrive the temperature
        ZeroMemory( &temperature, sizeof( NV_GPU_THERMAL_SETTINGS ) );
        temperature.version = NV_GPU_THERMAL_SETTINGS_VER;
        nvstatus = NvAPI_GPU_GetThermalSettings( nvGPUHandles[0], NVAPI_THERMAL_TARGET_ALL, &temperature );
        if( nvstatus != NVAPI_OK )
        {
            NvAPI_GetErrorMessage( nvstatus, string );
            DISPDBG( KE_ERROR, "\n\tNvAPI_GPU_GetThermalSettings(): " << string );
        }
        
        if( temperature.count == 0 )
        {
            DISPDBG( KE_WARNING, "temperature.count == 0" );
        }
        
        status->temperature = temperature.sensor[0].currentTemp;
#endif
    }
    
#ifdef _WIN32
    HMODULE hmod;
    
    NvAPI_QueryInterface_t      _NvAPI_QueryInterface;
    NvAPI_Initialize_t          _NvAPI_Initialize;
    NvAPI_EnumPhysicalGPUs_t    _NvAPI_EnumPhysicalGPUs;
    NvAPI_GPU_GetUsages_t       _NvAPI_GPU_GetUsages;
#endif
};





/*
 * D3DKMT GPU utilities (Intel)
 */

#if defined(_WIN32)
#include <d3dkmt.h>
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



PFND3DKMT_QUERYSTATISTICS	pfnD3DKMTQueryStatistics = nullptr;
HMODULE						hGdi = nullptr;
UINT						AdapterNumber = 0;
std::vector<float>			GpuUsage;
UINT64_DELTA				ClockTotalRunningTimeDelta = {0};
LARGE_INTEGER				ClockTotalRunningTimeFrequency = {0};
UINT64_DELTA				GpuTotalRunningTimeDelta = {0};
UINT64_DELTA				GpuSystemRunningTimeDelta = {0};
UINT64_DELTA				GpuNodesTotalRunningTimeDelta[16];
#endif


/*
 * Intel (D3DKMT) GPU monitor
 */
struct IKeGpuMonitorIntel : public IKeGpuMonitor
{
    /*
     * Name: IKeGpuMonitorIntel::Destroy()
     * Desc: If a handle to the gdi32.dll exists, close it.
     */
    KEMETHOD Destroy()
    {
#ifdef _WIN32
        if( hGdi )
        {
            CloseHandle( hGdi );
            hGdi = nullptr;
        }
#endif
    }
    
    /*
     * Name: IKeGpuMonitorIntel::Detect()
     * Desc: Detects the existence of an Intel based GPU.
     */
    _KEMETHOD(bool) Detect()
    {
#ifdef _WIN32
        /* TODO: Implement display adapter enumeration using
           EnumDisplay or whatever for Windows */
        
        /* Load the gdi32.dll directly, and get a function pointer to the necessary function. */
#ifdef _UWP
		hGdi = ::LoadPackagedLibrary( L"gdi32.dll", 0 );
#else
        hGdi = ::LoadLibrary( TEXT( "gdi32.dll" ) );
#endif
        if( !hGdi )
        {
            DISPDBG( KE_ERROR, "Unable to open gdi32.dll!" );
            return false;
        }
        
        pfnD3DKMTQueryStatistics = (PFND3DKMT_QUERYSTATISTICS) GetProcAddress( hGdi, "D3DKMTQueryStatistics" );
        if( !pfnD3DKMTQueryStatistics )
        {
            DISPDBG( KE_ERROR, "Unable to locate D3DKMTQueryStatistics()!" );
            return false;
        }
        
        FreeLibrary( hGdi );
        
        return true;
#endif
        
        return false;
    }
    
    /*
     * Name: IKeGpuMonitorIntel::GetDescription
     * Desc: Returns information that describes this GPU, such as the
     *       vendor and device IDs, total amount of VRAM, etc.
     */
    KEMETHOD GetDescription( KeGpuDescriptor* desc )
    {
        /* TODO: Implement for Windows and Linux */
    }
    
    /*
     * Name: IKeGpuMonitorIntel::GetStatus
     * Desc: Returns the GPU's various stats, such as GPU load percentage,
     *       current temperature, available video memory, as well as the
     *       current highest/lowest values of each attribute.
     * NOTE: None of the above stats are guaranteed to be available for
     *       Intel GPUs (under Windows) either.  The APIs to do this are
     *       not very well documented, and considering that Intel's HD
     *       series GPUs are now part of the CPU, there does not seem
     *       to be an accurate way to get the actual GPU temperature.
     */
    KEMETHOD GetStatus( KeGpuStatus* status )
    {
		/* TODO: Get adapter LUID and description without DirectX */
#if 0 //#ifdef _WIN32
        UINT64 SharedBytesUsed = 0, DedicatedBytesUsed = 0, CommittedBytesUsed = 0;
        D3DKMT_QUERYSTATISTICS QueryStatistics;
        ULONG i;
        ULONG64 TotalRunningTime = 0, SystemRunningTime = 0;
        UINT NodeCount = PVT_GetNodeCount();
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
        
        status->usage = int( GpuUsage * 100.0f );
#endif
    }
    
    /*
     * Name: KmtGetNodeCount
     * Desc: Returns the number of GPU nodes associated with this GPU.
     */
#ifdef _WIN32   // Windows only
    UINT PVT_GetNodeCount()
    {
#if 0 /* TODO: Get adapter LUID and description without DirectX */
        D3DKMT_QUERYSTATISTICS QueryStatistics;
        memset( &QueryStatistics, 0, sizeof( D3DKMT_QUERYSTATISTICS ) );
        QueryStatistics.Type = D3DKMT_QUERYSTATISTICS_ADAPTER;
        QueryStatistics.AdapterLuid = DxgiDesc.AdapterLuid;
        
        NTSTATUS status = pfnD3DKMTQueryStatistics( &QueryStatistics );
        if( !NT_SUCCESS( status ) )
            return 0;
        
        return QueryStatistics.QueryResult.AdapterInformation.NodeCount;
#else
		return 0;
#endif
    }
#endif
};



/*
 * Name: KmtDetectGpu
 * Desc: Uses Direct3D KMT to detect an Intel GPU for Whitney devices.  If the Intel
 *		 GPU hardware is not found, then this function returns a failure.  Once the
 *		 expected GPU is found, we open up GDI32.dll and get a function pointer to the
 *		 function required to monitor GPU specific stats.
 */
#if 0
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
#endif

/*
 * Name: KmtGetGpuUsage
 * Desc: Returns the GPU usage in the form of a percentage.  
 */
#if 0
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
#endif

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

//#endif // D3DKMT && _WIN32


