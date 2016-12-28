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