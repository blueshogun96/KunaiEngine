#pragma once

#define KE_ENABLE_NVAPI
#define KE_ENABLE_D3DKMT


/*
 * Name: NvGpuDetected
 * Desc: Returns true if supported Gpu has been detected on this system.
 * NOTE: This function is heavily OS dependent and is not guaranteed to
 *		 have support for your given Gpu.  
 */
BOOL KeGpuDetected();

/*
 * Name: NvGetGpuLoad
 * Desc: Returns the Gpu's current load percentage if available.
 */
int  KeGetGpuLoad();

/*
 * Name: NvGetGpuTemperature
 * Desc: Returns the current temperature of the Gpu if available.
 */
int  KeGetGpuTemperature();


/*
 * GPU descriptor 
 */
struct KeGpuDescriptor
{
    int         number;
    uint32_t    device_id;
    uint32_t    vendor_id;
    uint32_t    maximum_vram;
};

/*
 * GPU status
 */
struct KeGpuStatus
{
    int         usage;
    int         usage_highest;
    int         usage_lowwest;
    int         temperature;
    int         temperature_highest;
    int         temperature_lowest;
    uint32_t    available_vram;
    uint32_t    available_vram_highest;
    uint32_t    available_vram_losest;
};

/*
 * KeGpuMonitor class
 */
struct IKeGpuMonitor : public IKeUnknown
{
    KEMETHOD Destroy() PURE;

    _KEMETHOD(bool) Detect() PURE;
    KEMETHOD GetDescription( KeGpuDescriptor* desc ) PURE;
    KEMETHOD GetStatus( KeGpuStatus* status ) PURE;
};

bool KeCreateGpuMonitor( uint32_t flags, IKeGpuMonitor* monitor );
