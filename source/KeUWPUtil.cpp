//
//  ke_win32_util.cpp
//
//  Created by Shogun3D on 8/21/14.
//  Copyright (c) 2014 Shogun3D. All rights reserved.
//

#include "KePlatform.h"
#include "KeUWPUtil.h"
#include "KeDebug.h"
#include <ShlObj.h>
#include <tchar.h>

#include "InstructionSet.h"

const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;


/*
 * Name: KeSetCurrentPathToResourceDirectory
 * Desc: Sets the current path to the resource directory of the app.
 */
int KeSetCurrentPathToResourceDirectory()
{
	DISPDBG( KE_WARNING, "Resource path redirection not yet supported...\n" );

	return 0;
}

int KeGetCurrentPathToResourceDirectory(char* resource_path)
{
	DISPDBG( KE_WARNING, "Resource path redirection not yet supported...\n" );

	return 0;
}

/*
 * Name: KeIsOnlyInstance
 * Desc: Returns true if this is the only instance of the application running.
 * NOTE: This is generally only relevant on desktop OSes.
 */
bool KeIsOnlyInstance( const char* title )
{
	/* TODO: Implement */
	DISPDBG( KE_WARNING, "Not yet implemented for UWP!" );

	return true;
}

/*
 * Name: KeGetDiskSpace
 * Desc: Returns the amount of space free on the current disk drive.
 */
uint64_t KeGetDiskSpace()
{
	/* TODO: Implement */
	DISPDBG( KE_WARNING, "Not yet implemented for UWP!" );

	return 0;
}

/*
 * Name: KeGetCpuSpeed
 * Desc: Returns this machine's current CPU speed.
 */
uint64_t KeGetCpuSpeed()
{
    /* TODO: Implement */
	DISPDBG( KE_WARNING, "Not yet implemented for UWP!" );

	return 0;
}


/*
 * Name: KeGetCpuCount()
 * Desc: Returns the number of physical CPU cores on this machine.
 */
int KeGetCpuCount()
{
    /* Source: http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine */
    
    SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	return sysinfo.dwNumberOfProcessors;
}

/*
 * Name: KeGetCpuCapabilities
 * Desc: Checks this CPU's capabilities via CPUID (for x86)
 */
#define BOOL2STR(b) (b?" Yes":" No")
bool KeGetCpuCapabilities()
{
	std::stringstream sstr;
	sstr << KeGetCpuCount();
    sstr << "\n\tVendor: " <<  InstructionSet::Vendor() <<
		"\n\tBrand: " <<	   InstructionSet::Brand() <<
		"\n\tProcessors: " <<  KeGetCpuCount() <<
    
	    "\n\t\t3DNOW      " << BOOL2STR(InstructionSet::_3DNOW()) <<
        "\n\t\t3DNOWEXT   " << BOOL2STR(InstructionSet::_3DNOWEXT()) <<
        "\n\t\tABM        " << BOOL2STR(InstructionSet::ABM()) <<
        "\n\t\tADX        " << BOOL2STR(InstructionSet::ADX()) <<
        "\n\t\tAES        " << BOOL2STR(InstructionSet::AES()) <<
        "\n\t\tAVX        " << BOOL2STR(InstructionSet::AVX()) <<
        "\n\t\tAVX2       " << BOOL2STR(InstructionSet::AVX2()) <<
        "\n\t\tAVX512CD   " << BOOL2STR(InstructionSet::AVX512CD()) <<
        "\n\t\tAVX512ER   " << BOOL2STR(InstructionSet::AVX512ER()) <<
        "\n\t\tAVX512F    " << BOOL2STR(InstructionSet::AVX512F()) <<
        "\n\t\tAVX512PF   " << BOOL2STR(InstructionSet::AVX512PF()) <<
        "\n\t\tBMI1       " << BOOL2STR(InstructionSet::BMI1()) <<
        "\n\t\tBMI2       " << BOOL2STR(InstructionSet::BMI2()) <<
        "\n\t\tCLFSH      " << BOOL2STR(InstructionSet::CLFSH()) <<
        "\n\t\tCMPXCHG16B " << BOOL2STR(InstructionSet::CMPXCHG16B()) <<
        "\n\t\tCX8        " << BOOL2STR(InstructionSet::CX8()) <<
        "\n\t\tERMS       " << BOOL2STR(InstructionSet::ERMS()) <<
        "\n\t\tF16C       " << BOOL2STR(InstructionSet::F16C()) <<
        "\n\t\tFMA        " << BOOL2STR(InstructionSet::FMA()) <<
        "\n\t\tFSGSBASE   " << BOOL2STR(InstructionSet::FSGSBASE()) <<
        "\n\t\tFXSR       " << BOOL2STR(InstructionSet::FXSR()) <<
        "\n\t\tHLE        " << BOOL2STR(InstructionSet::HLE()) <<
        "\n\t\tINVPCID    " << BOOL2STR(InstructionSet::INVPCID()) <<
        "\n\t\tLAHF       " << BOOL2STR(InstructionSet::LAHF()) <<
        "\n\t\tLZCNT      " << BOOL2STR(InstructionSet::LZCNT()) <<
        "\n\t\tMMX        " << BOOL2STR(InstructionSet::MMX()) <<
        "\n\t\tMMXEXT     " << BOOL2STR(InstructionSet::MMXEXT()) <<
        "\n\t\tMONITOR    " << BOOL2STR(InstructionSet::MONITOR()) <<
        "\n\t\tMOVBE      " << BOOL2STR(InstructionSet::MOVBE()) <<
        "\n\t\tMSR        " << BOOL2STR(InstructionSet::MSR()) <<
        "\n\t\tOSXSAVE    " << BOOL2STR(InstructionSet::OSXSAVE()) <<
        "\n\t\tPCLMULQDQ  " << BOOL2STR(InstructionSet::PCLMULQDQ()) <<
        "\n\t\tPOPCNT     " << BOOL2STR(InstructionSet::POPCNT()) <<
        "\n\t\tPREFETCHWT1" << BOOL2STR(InstructionSet::PREFETCHWT1()) <<
        "\n\t\tRDRAND     " << BOOL2STR(InstructionSet::RDRAND()) <<
        "\n\t\tRDSEED     " << BOOL2STR(InstructionSet::RDSEED()) <<
        "\n\t\tRDTSCP     " << BOOL2STR(InstructionSet::RDTSCP()) <<
        "\n\t\tRTM        " << BOOL2STR(InstructionSet::RTM()) <<
        "\n\t\tSEP        " << BOOL2STR(InstructionSet::SEP()) <<
        "\n\t\tSHA        " << BOOL2STR(InstructionSet::SHA()) <<
        "\n\t\tSSE        " << BOOL2STR(InstructionSet::SSE()) <<
        "\n\t\tSSE2       " << BOOL2STR(InstructionSet::SSE2()) <<
        "\n\t\tSSE3       " << BOOL2STR(InstructionSet::SSE3()) <<
        "\n\t\tSSE4.1     " << BOOL2STR(InstructionSet::SSE41()) <<
        "\n\t\tSSE4.2     " << BOOL2STR(InstructionSet::SSE42()) <<
        "\n\t\tSSE4a      " << BOOL2STR(InstructionSet::SSE4a()) <<
        "\n\t\tSSSE3      " << BOOL2STR(InstructionSet::SSSE3()) <<
        "\n\t\tSYSCALL    " << BOOL2STR(InstructionSet::SYSCALL()) <<
        "\n\t\tTBM        " << BOOL2STR(InstructionSet::TBM()) <<
        "\n\t\tXOP        " << BOOL2STR(InstructionSet::XOP()) <<
        "\n\t\tXSAVE      " << BOOL2STR(InstructionSet::XSAVE()) <<
	    "\n";

	/*while( i < extension_count )
    {
		ext_str += "\t\t";
        ext_str += (const char*) glGetStringi( GL_EXTENSIONS, i );
		ext_str += "\n";
        i++;
    }*/

	DISPDBG( KE_DBGLVL(0), sstr.str() );

	return true;
}


/*
 * Name: KeGetPhysicalMemoryStatus
 * Desc: Returns the amount of physical memory installed and the amount that is currently used
 */
int KeGetPhysicalMemoryStatus( uint64_t* total, uint64_t* free )
{
	MEMORYSTATUSEX status;

	if( !GlobalMemoryStatusEx( &status ) )
		return 0;

	*total = status.ullTotalPhys;
	*free = status.ullAvailPhys;

    return 1;
}

/*
 * Name: KeGetVirtualMemoryStatus
 * Desc: Returns the amount of virtual memory installed and the amount that is currently used
 */
int KeGetVirtualMemoryStatus( uint64_t* total, uint64_t* free )
{
	MEMORYSTATUSEX status;

	if( !GlobalMemoryStatusEx( &status ) )
		return 0;

	*total = status.ullTotalVirtual;
	*free = status.ullAvailVirtual;

    return 1;
}

const char* KeGetSaveGameDirectory( const char* game_app_directory )
{
	/* TODO: Implement */
	DISPDBG( KE_WARNING, "Not yet implemented for UWP!" );

	return "./";
}

void KeRequestUserAttention()
{
	/* TODO: Implement... if possible */
	DISPDBG( KE_WARNING, "Not yet implemented for UWP!" );
}


